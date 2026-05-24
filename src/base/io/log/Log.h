/* XMRig
 * Copyright (c) 2019      Spudz76     <https://github.com/Spudz76>
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

#ifndef RXS_LOG_H
#define RXS_LOG_H


#include <cstddef>
#include <cstdint>


namespace rxs {


class ILogBackend;
class LogPrivate;


class Log
{
public:
    enum Level : int {
        NONE = -1,
        EMERG,   // system is unusable
        ALERT,   // action must be taken immediately
        CRIT,    // critical conditions
        ERR,     // error conditions
        WARNING, // warning conditions
        NOTICE,  // normal but significant condition
        INFO,    // informational
        DEBUG,   // debug-level messages
    };

    constexpr static size_t kMaxBufferSize = 16384;

    static void add(ILogBackend *backend);
    static void destroy();
    static void init();
    static void print(const char *fmt, ...);
    static void print(Level level, const char *fmt, ...);

    static inline bool isBackground()                   { return m_background; }
    static inline bool isColors()                       { return m_colors; }
    static inline bool isVerbose()                      { return m_verbose > 0; }
    static inline uint32_t verbose()                    { return m_verbose; }
    static inline void setBackground(bool background)   { m_background = background; }
    static inline void setColors(bool colors)           { m_colors = colors; }
    static inline void setVerbose(uint32_t verbose)     { m_verbose = verbose; }

private:
    static bool m_background;
    static bool m_colors;
    static LogPrivate *d;
    static uint32_t m_verbose;
};


#define CSI                 "\x1B["     // Control Sequence Introducer (ANSI spec name)
#define CLEAR               CSI "0m"    // all attributes off

#define BRIGHT_BLACK_S      CSI "38;5;245m"
#define BLACK_S             CSI "38;5;240m"
#define BLACK_BOLD_S        CSI "1;38;5;246m"
#define RED_S               CSI "38;5;167m" 
#define RED_BOLD_S          CSI "1;38;5;203m"
#define GREEN_S             CSI "38;5;108m"
#define GREEN_BOLD_S        CSI "1;38;5;114m"
#define YELLOW_S            CSI "38;5;179m"
#define YELLOW_BOLD_S       CSI "1;38;5;220m"
#define BLUE_S              CSI "38;5;75m"
#define BLUE_BOLD_S         CSI "1;38;5;39m"
#define MAGENTA_S           CSI "38;5;176m"
#define MAGENTA_BOLD_S      CSI "1;38;5;207m"
#define CYAN_S              CSI "38;5;73m"
#define CYAN_BOLD_S         CSI "1;38;5;87m"
#define WHITE_S             CSI "38;5;249m"
#define WHITE_BOLD_S        CSI "1;38;5;231m"

#define RED_BG_BOLD_S       CSI "1;48;5;124m"
#define GREEN_BG_BOLD_S     CSI "1;48;5;22m"
#define YELLOW_BG_BOLD_S    CSI "1;48;5;58m"
#define BLUE_BG_S           CSI "48;5;17m"
#define BLUE_BG_BOLD_S      CSI "1;48;5;18m"
#define MAGENTA_BG_S        CSI "48;5;53m"
#define MAGENTA_BG_BOLD_S   CSI "1;48;5;90m"
#define CYAN_BG_S           CSI "48;5;23m"
#define CYAN_BG_BOLD_S      CSI "1;48;5;29m"

#define BLACK(x)            BLACK_S x CLEAR
#define BLACK_BOLD(x)       BLACK_BOLD_S x CLEAR
#define RED(x)              RED_S x CLEAR
#define RED_BOLD(x)         RED_BOLD_S x CLEAR
#define GREEN(x)            GREEN_S x CLEAR
#define GREEN_BOLD(x)       GREEN_BOLD_S x CLEAR
#define YELLOW(x)           YELLOW_S x CLEAR
#define YELLOW_BOLD(x)      YELLOW_BOLD_S x CLEAR
#define BLUE(x)             BLUE_S x CLEAR
#define BLUE_BOLD(x)        BLUE_BOLD_S x CLEAR
#define MAGENTA(x)          MAGENTA_S x CLEAR
#define MAGENTA_BOLD(x)     MAGENTA_BOLD_S x CLEAR
#define CYAN(x)             CYAN_S x CLEAR
#define CYAN_BOLD(x)        CYAN_BOLD_S x CLEAR
#define WHITE(x)            WHITE_S x CLEAR
#define WHITE_BOLD(x)       WHITE_BOLD_S x CLEAR

#define RED_BG_BOLD(x)      RED_BG_BOLD_S x CLEAR
#define GREEN_BG_BOLD(x)    GREEN_BG_BOLD_S x CLEAR
#define YELLOW_BG_BOLD(x)   YELLOW_BG_BOLD_S x CLEAR
#define BLUE_BG(x)          BLUE_BG_S x CLEAR
#define BLUE_BG_BOLD(x)     BLUE_BG_BOLD_S x CLEAR
#define MAGENTA_BG(x)       MAGENTA_BG_S x CLEAR
#define MAGENTA_BG_BOLD(x)  MAGENTA_BG_BOLD_S x CLEAR
#define CYAN_BG(x)          CYAN_BG_S x CLEAR
#define CYAN_BG_BOLD(x)     CYAN_BG_BOLD_S x CLEAR


#define LOG_EMERG(x, ...)   rxs::Log::print(rxs::Log::EMERG,   x, ##__VA_ARGS__)
#define LOG_ALERT(x, ...)   rxs::Log::print(rxs::Log::ALERT,   x, ##__VA_ARGS__)
#define LOG_CRIT(x, ...)    rxs::Log::print(rxs::Log::CRIT,    x, ##__VA_ARGS__)
#define LOG_ERR(x, ...)     rxs::Log::print(rxs::Log::ERR,     x, ##__VA_ARGS__)
#define LOG_WARN(x, ...)    rxs::Log::print(rxs::Log::WARNING, x, ##__VA_ARGS__)
#define LOG_NOTICE(x, ...)  rxs::Log::print(rxs::Log::NOTICE,  x, ##__VA_ARGS__)
#define LOG_INFO(x, ...)    rxs::Log::print(rxs::Log::INFO,    x, ##__VA_ARGS__)
#define LOG_V1(x, ...)      if (rxs::Log::verbose() > 0) { rxs::Log::print(rxs::Log::INFO, x, ##__VA_ARGS__); }
#define LOG_VERBOSE         LOG_V1
#define LOG_V2(x, ...)      if (rxs::Log::verbose() > 1) { rxs::Log::print(rxs::Log::INFO, x, ##__VA_ARGS__); }
#define LOG_V3(x, ...)      if (rxs::Log::verbose() > 2) { rxs::Log::print(rxs::Log::INFO, x, ##__VA_ARGS__); }
#define LOG_V4(x, ...)      if (rxs::Log::verbose() > 3) { rxs::Log::print(rxs::Log::INFO, x, ##__VA_ARGS__); }
#define LOG_V5(x, ...)      if (rxs::Log::verbose() > 4) { rxs::Log::print(rxs::Log::INFO, x, ##__VA_ARGS__); }

#ifdef APP_DEBUG
#   define LOG_DEBUG(x, ...) rxs::Log::print(rxs::Log::DEBUG, x, ##__VA_ARGS__)
#else
#   define LOG_DEBUG(x, ...)
#endif

#if defined(APP_DEBUG) || defined(APP_DEVEL)
#   define LOG_DEBUG_ERR(x, ...)  rxs::Log::print(rxs::Log::ERR,     x, ##__VA_ARGS__)
#   define LOG_DEBUG_WARN(x, ...) rxs::Log::print(rxs::Log::WARNING, x, ##__VA_ARGS__)
#else
#   define LOG_DEBUG_ERR(x, ...)
#   define LOG_DEBUG_WARN(x, ...)
#endif


} /* namespace rxs */


#endif /* RXS_LOG_H */
