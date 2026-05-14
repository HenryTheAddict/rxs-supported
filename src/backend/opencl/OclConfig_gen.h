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

#ifndef RXS_OCLCONFIG_GEN_H
#define RXS_OCLCONFIG_GEN_H


#include "backend/common/Threads.h"
#include "backend/opencl/OclThreads.h"


#include <algorithm>


namespace rxs {


static inline size_t generate(const char *key, Threads<OclThreads> &threads, const Algorithm &algorithm, const std::vector<OclDevice> &devices)
{
    if (threads.isExist(algorithm) || threads.has(key)) {
        return 0;
    }

    return threads.move(key, OclThreads(devices, algorithm));
}


template<Algorithm::Family FAMILY>
static inline size_t generate(Threads<OclThreads> &, const std::vector<OclDevice> &) { return 0; }












static inline std::vector<OclDevice> filterDevices(const std::vector<OclDevice> &devices, const std::vector<uint32_t> &hints)
{
    std::vector<OclDevice> out;
    out.reserve(std::min(devices.size(), hints.size()));

    for (const auto &device  : devices) {
        auto it = std::find(hints.begin(), hints.end(), device.index());
        if (it != hints.end()) {
            out.emplace_back(device);
        }
    }

    return out;
}


} /* namespace rxs */


#endif /* RXS_OCLCONFIG_GEN_H */
