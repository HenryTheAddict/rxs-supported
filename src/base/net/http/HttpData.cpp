/* XMRig
 * Copyright (c) 2014-2019 heapwolf    <https://github.com/heapwolf>
 * Copyright (c) 2018-2025 SChernykh   <https://github.com/SChernykh>
 * Copyright (c) 2016-2025 XMRig       <https://github.com/xmrig>, <support@xmrig.com>
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

#include "base/net/http/HttpData.h"
#include "3rdparty/llhttp/llhttp.h"
#include "3rdparty/rapidjson/document.h"
#include "3rdparty/rapidjson/error/en.h"
#include "base/io/json/Json.h"


#include <uv.h>
#include <stdexcept>


namespace rxs {


const std::string HttpData::kApplicationJson    = "application/json";
const std::string HttpData::kContentType        = "Content-Type";
const std::string HttpData::kContentTypeL       = "content-type";
const std::string HttpData::kTextPlain          = "text/plain";


} // namespace rxs


bool rxs::HttpData::isJSON() const
{
    const auto it = headers.find(kContentTypeL);
    if (it == headers.end()) {
        return false;
    }

    return it->second == kApplicationJson || it->second == kTextPlain;
}


const char *rxs::HttpData::methodName() const
{
    return llhttp_method_name(static_cast<llhttp_method>(method));
}


rapidjson::Document rxs::HttpData::json() const
{
    if (status < 0) {
        throw std::runtime_error(statusName());
    }

    if (!isJSON()) {
        throw std::runtime_error("the response is not a valid JSON response");
    }

    using namespace rapidjson;
    Document doc;
    if (doc.Parse(body.c_str()).HasParseError()) {
        throw std::runtime_error(GetParseError_En(doc.GetParseError()));
    }

    if (doc.IsObject() && !doc.ObjectEmpty()) {
        const char *error = Json::getString(doc, "error");
        if (error) {
            throw std::runtime_error(error);
        }
    }

    return doc;
}


const char *rxs::HttpData::statusName(int status)
{
    if (status < 0) {
        return uv_strerror(status);
    }

    return llhttp_status_name(static_cast<llhttp_status>(status));
}
