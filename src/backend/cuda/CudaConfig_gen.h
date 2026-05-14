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

#ifndef XMRIG_CUDACONFIG_GEN_H
#define XMRIG_CUDACONFIG_GEN_H


#include "backend/common/Threads.h"
#include "backend/cuda/CudaThreads.h"
#include "backend/cuda/wrappers/CudaDevice.h"


#include <algorithm>


namespace xmrig {


static inline size_t generate(const char *key, Threads<CudaThreads> &threads, const Algorithm &algorithm, const std::vector<CudaDevice> &devices)
{
    if (threads.isExist(algorithm) || threads.has(key)) {
        return 0;
    }

    return threads.move(key, CudaThreads(devices, algorithm));
}


template<Algorithm::Family FAMILY>
static inline size_t generate(Threads<CudaThreads> &, const std::vector<CudaDevice> &) { return 0; }












} /* namespace xmrig */


#endif /* XMRIG_CUDACONFIG_GEN_H */
