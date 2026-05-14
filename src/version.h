/* XMRig
 * Copyright (c) 2018-2026 SChernykh   <https://github.com/SChernykh>
 * Copyright (c) 2016-2026 XMRig       <https://github.com/xmrig>, <support@xmrig.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef RXS_VERSION_H
#define RXS_VERSION_H

#define APP_ID        "rxs"
#define APP_NAME      "rxs"
#define APP_DESC      "RandomX Miner"
#define APP_VERSION   "0.0.1"
#define APP_DOMAIN    ""
#define APP_SITE      ""
#define APP_COPYRIGHT "Copyright (C) 2016-2026 xmrig.com"
#define APP_KIND      "miner"

#define APP_VER_MAJOR 0
#define APP_VER_MINOR 0
#define APP_VER_PATCH 1

#ifdef _MSC_VER
#   if (_MSC_VER >= 1950)
#       define MSVC_VERSION 2026
#   elif (_MSC_VER >=1930 && _MSC_VER < 1950)
#       define MSVC_VERSION 2022
#   elif (_MSC_VER >= 1920 && _MSC_VER < 1930)
#       define MSVC_VERSION 2019
#   elif (_MSC_VER >= 1910 && _MSC_VER < 1920)
#       define MSVC_VERSION 2017
#   elif _MSC_VER == 1900
#       define MSVC_VERSION 2015
#   elif _MSC_VER == 1800
#       define MSVC_VERSION 2013
#   elif _MSC_VER == 1700
#       define MSVC_VERSION 2012
#   elif _MSC_VER == 1600
#       define MSVC_VERSION 2010
#   else
#       define MSVC_VERSION 0
#   endif
#endif

#ifdef RXS_OS_WIN
#    define APP_OS "Windows"
#elif defined RXS_OS_IOS
#    define APP_OS "iOS"
#elif defined RXS_OS_MACOS
#    define APP_OS "macOS"
#elif defined RXS_OS_ANDROID
#    define APP_OS "Android"
#elif defined RXS_OS_LINUX
#    define APP_OS "Linux"
#elif defined RXS_OS_FREEBSD
#    define APP_OS "FreeBSD"
#elif defined RXS_OS_OPENBSD
#    define APP_OS "OpenBSD"
#elif defined RXS_OS_HAIKU
#    define APP_OS "Haiku"
#else
#    define APP_OS "Unknown OS"
#endif

#define STR(X) #X
#define STR2(X) STR(X)

#ifdef RXS_ARM
#   define APP_ARCH "ARMv" STR2(RXS_ARM)
#elif defined(RXS_RISCV)
#   define APP_ARCH "RISC-V"
#else
#   if defined(__x86_64__) || defined(__amd64__) || defined(_M_X64) || defined(_M_AMD64)
#       define APP_ARCH "x86-64"
#   else
#       define APP_ARCH "x86"
#   endif
#endif

#ifdef RXS_64_BIT
#   define APP_BITS "64 bit"
#else
#   define APP_BITS "32 bit"
#endif

#endif // RXS_VERSION_H
