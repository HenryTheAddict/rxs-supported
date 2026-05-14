/* XMRig
 * Copyright (c) 2018      Lee Clagett <https://github.com/vtnerd>
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

#include "base/crypto/Algorithm.h"
#include "3rdparty/rapidjson/document.h"
#include "base/tools/String.h"


#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <map>


#ifdef _MSC_VER
#   define strcasecmp  _stricmp
#endif


namespace xmrig {


const char *Algorithm::kINVALID   = "invalid";
const char *Algorithm::kRX        = "rx";
const char *Algorithm::kRX_0      = "rx/0";
const char *Algorithm::kRX_V2     = "rx/2";
const char *Algorithm::kRX_WOW    = "rx/wow";
const char *Algorithm::kRX_ARQ    = "rx/arq";
const char *Algorithm::kRX_GRAFT  = "rx/graft";
const char *Algorithm::kRX_SFX    = "rx/sfx";
const char *Algorithm::kRX_YADA   = "rx/yada";


#define ALGO_NAME(ALGO)         { Algorithm::ALGO, Algorithm::k##ALGO }
#define ALGO_ALIAS(ALGO, NAME)  { NAME, Algorithm::ALGO }
#define ALGO_ALIAS_AUTO(ALGO)   { Algorithm::k##ALGO, Algorithm::ALGO }


static const std::map<uint32_t, const char *> kAlgorithmNames = {
    ALGO_NAME(RX_0),
    ALGO_NAME(RX_V2),
    ALGO_NAME(RX_WOW),
    ALGO_NAME(RX_ARQ),
    ALGO_NAME(RX_GRAFT),
    ALGO_NAME(RX_SFX),
    ALGO_NAME(RX_YADA),
};


struct aliasCompare
{
   inline bool operator()(const char *a, const char *b) const   { return strcasecmp(a, b) < 0; }
};


static const std::map<const char *, Algorithm::Id, aliasCompare> kAlgorithmAliases = {
    ALGO_ALIAS_AUTO(RX_0),      ALGO_ALIAS(RX_0,    "randomx"),
                                ALGO_ALIAS(RX_0,    "rx"),
    ALGO_ALIAS_AUTO(RX_V2),
    ALGO_ALIAS_AUTO(RX_WOW),    ALGO_ALIAS(RX_WOW,  "randomwow"),
    ALGO_ALIAS_AUTO(RX_ARQ),    ALGO_ALIAS(RX_ARQ,  "randomarq"),
    ALGO_ALIAS_AUTO(RX_GRAFT),  ALGO_ALIAS(RX_GRAFT,"randomgraft"),
    ALGO_ALIAS_AUTO(RX_SFX),    ALGO_ALIAS(RX_SFX,  "randomsfx"),
    ALGO_ALIAS_AUTO(RX_YADA),   ALGO_ALIAS(RX_YADA, "randomyada"),
};


Algorithm::Algorithm(const rapidjson::Value &value) : m_id(parse(value.IsString() ? value.GetString() : nullptr))
{
}


Algorithm::Algorithm(uint32_t id) : m_id(static_cast<Id>(id))
{
    assert(isValid());
}


const char *Algorithm::name() const
{
    auto it = kAlgorithmNames.find(m_id);
    if (it != kAlgorithmNames.end()) {
        return it->second;
    }

    return kINVALID;
}


rapidjson::Value Algorithm::toJSON() const
{
    return rapidjson::Value(rapidjson::StringRef(name()));
}


rapidjson::Value Algorithm::toJSON(rapidjson::Document &doc) const
{
    return rapidjson::Value(name(), doc.GetAllocator());
}


Algorithm::Id Algorithm::parse(const char *name)
{
    if (name == nullptr || strlen(name) < 2) {
        return INVALID;
    }

    auto it = kAlgorithmAliases.find(name);
    if (it != kAlgorithmAliases.end()) {
        return it->second;
    }

    return INVALID;
}


size_t Algorithm::count()
{
    return kAlgorithmNames.size();
}


std::vector<Algorithm> Algorithm::all(const std::function<bool(const Algorithm &algo)> &filter)
{
    std::vector<Algorithm> out;
    out.reserve(kAlgorithmNames.size());

    for (const auto &kv : kAlgorithmNames) {
        Algorithm algo(kv.first);
        if (!filter || filter(algo)) {
            out.push_back(algo);
        }
    }

    return out;
}


} /* namespace xmrig */
