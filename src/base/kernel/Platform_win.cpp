/* XMRig
 * Copyright (c) 2016-2025 XMRig       <https://github.com/xmrig>, <support@xmrig.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <cstdio>
#include <limits>

#ifndef NOMINMAX
#   define NOMINMAX
#endif

#include <winsock2.h>
#include <windows.h>
#include <uv.h>


#include "base/kernel/Platform.h"
#include "version.h"


static inline OSVERSIONINFOEX winOsVersion()
{
    typedef NTSTATUS (NTAPI *RtlGetVersionFunction)(LPOSVERSIONINFO);
    OSVERSIONINFOEX result = { sizeof(OSVERSIONINFOEX), 0, 0, 0, 0, {'\0'}, 0, 0, 0, 0, 0 };

    HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
    if (ntdll) {
        auto rtlGetVersion = reinterpret_cast<RtlGetVersionFunction>(GetProcAddress(ntdll, "RtlGetVersion"));
        if (rtlGetVersion) {
            rtlGetVersion(reinterpret_cast<LPOSVERSIONINFO>(&result));
        }
    }

    return result;
}


char *rxs::Platform::createUserAgent()
{
    const auto osver = winOsVersion();
    constexpr const size_t max = 256;

    char *buf = new char[max]();
    int length = snprintf(buf, max, "%s/%s (Windows NT %lu.%lu", APP_NAME, APP_VERSION, osver.dwMajorVersion, osver.dwMinorVersion);

#   if defined(RXS_64_BIT)
    length += snprintf(buf + length, max - length, "; Win64; %s) libuv/%s", APP_ARCH, uv_version_string());
#   else
    length += snprintf(buf + length, max - length, ") libuv/%s", uv_version_string());
#   endif

#   ifdef __clang__
    snprintf(buf + length, max - length, " clang/%d.%d.%d", __clang_major__, __clang_minor__, __clang_patchlevel__);
#   elif defined(_MSC_VER)
    snprintf(buf + length, max - length, " msvc/%d", _MSC_VER);
#   endif

    return buf;
}


#ifndef RXS_FEATURE_HWLOC
bool rxs::Platform::setThreadAffinity(uint64_t cpu_id)
{
    if (cpu_id >= sizeof(DWORD_PTR) * 8) {
        return false;
    }

    const bool result = SetThreadAffinityMask(GetCurrentThread(), static_cast<DWORD_PTR>(1) << cpu_id) != 0;
    Sleep(1);

    return result;
}
#endif


void rxs::Platform::setProcessPriority(int priority)
{
    if (priority == -1) {
        return;
    }

    DWORD prio = IDLE_PRIORITY_CLASS;
    switch (priority) {
    case 1: prio = BELOW_NORMAL_PRIORITY_CLASS; break;
    case 2: prio = NORMAL_PRIORITY_CLASS; break;
    case 3: prio = ABOVE_NORMAL_PRIORITY_CLASS; break;
    case 4: prio = HIGH_PRIORITY_CLASS; break;
    case 5: prio = REALTIME_PRIORITY_CLASS; break;
    default: break;
    }

    SetPriorityClass(GetCurrentProcess(), prio);
}


void rxs::Platform::setThreadPriority(int priority)
{
    if (priority == -1) {
        return;
    }

    int prio = THREAD_PRIORITY_IDLE;
    switch (priority) {
    case 1: prio = THREAD_PRIORITY_BELOW_NORMAL; break;
    case 2: prio = THREAD_PRIORITY_NORMAL; break;
    case 3: prio = THREAD_PRIORITY_ABOVE_NORMAL; break;
    case 4: prio = THREAD_PRIORITY_HIGHEST; break;
    case 5: prio = THREAD_PRIORITY_TIME_CRITICAL; break;
    default: break;
    }

    SetThreadPriority(GetCurrentThread(), prio);
}


bool rxs::Platform::isOnBatteryPower()
{
    SYSTEM_POWER_STATUS status;
    if (GetSystemPowerStatus(&status)) {
        return status.ACLineStatus == 0;
    }

    return false;
}


uint64_t rxs::Platform::idleTime()
{
    LASTINPUTINFO info{};
    info.cbSize = sizeof(LASTINPUTINFO);

    if (!GetLastInputInfo(&info)) {
        return std::numeric_limits<uint64_t>::max();
    }

    return static_cast<uint64_t>(GetTickCount64() - info.dwTime);
}
