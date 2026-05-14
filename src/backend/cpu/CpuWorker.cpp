/* XMRig
 * Copyright (c) 2018-2021 SChernykh   <https://github.com/SChernykh>
 * Copyright (c) 2016-2021 XMRig       <https://github.com/xmrig>, <support@xmrig.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <cassert>
#include <thread>
#include <mutex>


#include "backend/cpu/Cpu.h"
#include "backend/cpu/CpuWorker.h"
#include "base/tools/Alignment.h"
#include "base/tools/Chrono.h"
#include "core/config/Config.h"
#include "core/Miner.h"
#include "crypto/common/Nonce.h"
#include "crypto/common/VirtualMemory.h"
#include "crypto/rx/Rx.h"
#include "crypto/rx/RxCache.h"
#include "crypto/rx/RxDataset.h"
#include "crypto/rx/RxVm.h"
#include "net/JobResults.h"
#include "crypto/randomx/randomx.h"


#ifdef XMRIG_FEATURE_BENCHMARK
#   include "backend/common/benchmark/BenchState.h"
#endif


namespace xmrig {

static constexpr uint32_t kReserveCount = 32768;

} // namespace xmrig


template<size_t N>
xmrig::CpuWorker<N>::CpuWorker(size_t id, const CpuLaunchData &data) :
    Worker(id, data.affinity, data.priority),
    m_algorithm(data.algorithm),
    m_assembly(data.assembly),
    m_hwAES(data.hwAES),
    m_yield(data.yield),
    m_miner(data.miner),
    m_threads(data.threads)
{
    m_memory = new VirtualMemory(m_algorithm.l3() * N, data.hugePages, false, true, node(), VirtualMemory::kDefaultHugePageSize);
}


template<size_t N>
xmrig::CpuWorker<N>::~CpuWorker()
{
    RxVm::destroy(m_vm);
    delete m_memory;
}


template<size_t N>
void xmrig::CpuWorker<N>::allocateRandomX_VM()
{
    RxDataset *dataset = Rx::dataset(m_job.currentJob(), node());

    while (dataset == nullptr) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        if (Nonce::sequence(Nonce::CPU) == 0) {
            return;
        }

        dataset = Rx::dataset(m_job.currentJob(), node());
    }

    if (!m_vm) {
        uint8_t* scratchpad = m_memory->isHugePages() ? m_memory->scratchpad() : dataset->tryAllocateScrathpad();
        m_vm = RxVm::create(dataset, scratchpad ? scratchpad : m_memory->scratchpad(), !m_hwAES, m_assembly, node());
    }
    else if (!dataset->get() && (m_job.currentJob().seed() != m_seed)) {
        randomx_vm_set_cache(m_vm, dataset->cache()->get());
    }
    m_seed = m_job.currentJob().seed();
}


template<size_t N>
bool xmrig::CpuWorker<N>::selfTest()
{
    if (m_algorithm.family() == Algorithm::RANDOM_X) {
        return N == 1;
    }
    return false;
}


template<size_t N>
void xmrig::CpuWorker<N>::hashrateData(uint64_t &hashCount, uint64_t &, uint64_t &rawHashes) const
{
    hashCount = m_count;
    rawHashes = m_count;
}


template<size_t N>
void xmrig::CpuWorker<N>::start()
{
    while (Nonce::sequence(Nonce::CPU) > 0) {
        if (Nonce::isPaused()) {
            do {
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
            while (Nonce::isPaused() && Nonce::sequence(Nonce::CPU) > 0);

            if (Nonce::sequence(Nonce::CPU) == 0) {
                break;
            }

            consumeJob();
        }

        bool first = true;
        alignas(64) uint64_t tempHash[8] = {};

        size_t prev_job_size = 0;
        alignas(64) uint8_t prev_job[Job::kMaxBlobSize] = {};

        while (!Nonce::isOutdated(Nonce::CPU, m_job.sequence())) {
            const Job &job = m_job.currentJob();

            if (job.algorithm().l3() != m_algorithm.l3()) {
                break;
            }

            uint32_t current_job_nonces[N];
            for (size_t i = 0; i < N; ++i) {
                current_job_nonces[i] = readUnaligned(m_job.nonce(i));
            }

#           ifdef XMRIG_FEATURE_BENCHMARK
            if (m_benchSize) {
                if (current_job_nonces[0] >= m_benchSize) {
                    return BenchState::done();
                }

                if (m_threads == 1) {
                    *(uint64_t*)(m_job.blob()) ^= BenchState::data();
                }
            }
#           endif

            uint8_t miner_signature_saved[64];
            uint8_t* miner_signature_ptr = m_job.blob() + m_job.nonceOffset() + m_job.nonceSize();

            if (first) {
                first = false;
                if (job.hasMinerSignature()) {
                    job.generateMinerSignature(m_job.blob(), job.size(), miner_signature_ptr);
                }
                randomx_calculate_hash_first(m_vm, tempHash, m_job.blob(), job.size());

                if (RandomX_CurrentConfig.Tweak_V2_COMMITMENT) {
                    prev_job_size = job.size();
                    memcpy(prev_job, m_job.blob(), prev_job_size);
                }
            }

            if (!nextRound()) {
                break;
            }

            if (job.hasMinerSignature()) {
                memcpy(miner_signature_saved, miner_signature_ptr, sizeof(miner_signature_saved));
                job.generateMinerSignature(m_job.blob(), job.size(), miner_signature_ptr);
            }

            randomx_calculate_hash_next(m_vm, tempHash, m_job.blob(), job.size(), m_hash);

            if (RandomX_CurrentConfig.Tweak_V2_COMMITMENT) {
                memcpy(m_commitment, m_hash, RANDOMX_HASH_SIZE);
                randomx_calculate_commitment(prev_job, prev_job_size, m_hash, m_hash);
                prev_job_size = job.size();
                memcpy(prev_job, m_job.blob(), prev_job_size);
            }

            for (size_t i = 0; i < N; ++i) {
                const uint64_t value = *reinterpret_cast<uint64_t*>(m_hash + (i * 32) + 24);

#               ifdef XMRIG_FEATURE_BENCHMARK
                if (m_benchSize) {
                    if (current_job_nonces[i] < m_benchSize) {
                        BenchState::add(value);
                    }
                }
                else
#               endif

                if (value < job.target()) {
                    uint8_t* extra_data = nullptr;

                    if (RandomX_CurrentConfig.Tweak_V2_COMMITMENT) {
                        extra_data = m_commitment;
                    }
                    else if (job.hasMinerSignature()) {
                        extra_data = miner_signature_saved;
                    }

                    JobResults::submit(job, current_job_nonces[i], m_hash + (i * 32), extra_data);
                }
            }
            m_count += N;

            if (m_yield) {
                std::this_thread::yield();
            }
        }

        if (!Nonce::isPaused()) {
            consumeJob();
        }
    }
}


template<size_t N>
bool xmrig::CpuWorker<N>::nextRound()
{
#   ifdef XMRIG_FEATURE_BENCHMARK
    const uint32_t count = m_benchSize ? 1U : kReserveCount;
#   else
    constexpr uint32_t count = kReserveCount;
#   endif

    if (!m_job.nextRound(count, 1)) {
        JobResults::done(m_job.currentJob());
        return false;
    }

    return true;
}


template<size_t N>
void xmrig::CpuWorker<N>::consumeJob()
{
    if (Nonce::sequence(Nonce::CPU) == 0) {
        return;
    }

    auto job = m_miner->job();

#   ifdef XMRIG_FEATURE_BENCHMARK
    m_benchSize          = job.benchSize();
    const uint32_t count = m_benchSize ? 1U : kReserveCount;
#   else
    constexpr uint32_t count = kReserveCount;
#   endif

    m_job.add(job, count, Nonce::CPU);

    allocateRandomX_VM();
}


namespace xmrig {

template class CpuWorker<1>;
template class CpuWorker<2>;
template class CpuWorker<3>;
template class CpuWorker<4>;
template class CpuWorker<5>;
template class CpuWorker<8>;

} // namespace xmrig
