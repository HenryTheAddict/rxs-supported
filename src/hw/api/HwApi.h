/* XMRig
 * Copyright (c) 2018-2021 SChernykh    <https://github.com/SChernykh>
 * Copyright (c) 2016-2021 XMRig        <https://github.com/xmrig>, <support@xmrig.com>
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

#ifndef RXS_HWAPI_H
#define RXS_HWAPI_H


#include "base/api/interfaces/IApiListener.h"


#include <memory>


namespace rxs {


class DmiReader;


class HwApi : public IApiListener
{
public:
    HwApi() = default;

protected:
    void onRequest(IApiRequest &request) override;

private:
#   ifdef RXS_FEATURE_DMI
    std::shared_ptr<DmiReader> m_dmi;
#   endif
};


} /* namespace rxs */


#endif /* RXS_HWAPI_H */
