/* XMRig
 * Copyright (c) 2016-2024 XMRig       <https://github.com/xmrig>, <support@xmrig.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef NOMINMAX
#   define NOMINMAX
#endif

#include <windows.h>


#include "crypto/common/VirtualMemory.h"


bool rxs::VirtualMemory::isHugepagesAvailable()
{
    return GetLargePageMinimum() > 0;
}


bool rxs::VirtualMemory::isOneGbPagesAvailable()
{
    return false;
}


bool rxs::VirtualMemory::protectRW(void *p, size_t size)
{
    DWORD oldProtect = 0;
    return VirtualProtect(p, size, PAGE_READWRITE, &oldProtect) != 0;
}


bool rxs::VirtualMemory::protectRWX(void *p, size_t size)
{
    DWORD oldProtect = 0;
    return VirtualProtect(p, size, PAGE_EXECUTE_READWRITE, &oldProtect) != 0;
}


bool rxs::VirtualMemory::protectRX(void *p, size_t size)
{
    DWORD oldProtect = 0;
    const bool result = VirtualProtect(p, size, PAGE_EXECUTE_READ, &oldProtect) != 0;
    flushInstructionCache(p, size);

    return result;
}


void *rxs::VirtualMemory::allocateExecutableMemory(size_t size, bool hugePages)
{
    if (hugePages) {
        void *mem = allocateLargePagesMemory(size);
        if (mem) {
            protectRWX(mem, align(size, hugePageSize()));
            return mem;
        }
    }

    return VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
}


void *rxs::VirtualMemory::allocateLargePagesMemory(size_t size)
{
    const size_t pageSize = hugePageSize();
    if (pageSize == 0) {
        return nullptr;
    }

    return VirtualAlloc(nullptr, align(size, pageSize), MEM_COMMIT | MEM_RESERVE | MEM_LARGE_PAGES, PAGE_READWRITE);
}


void *rxs::VirtualMemory::allocateOneGbPagesMemory(size_t)
{
    return nullptr;
}


bool rxs::VirtualMemory::adviseLargePages(void *, size_t)
{
    return false;
}


void rxs::VirtualMemory::flushInstructionCache(void *p, size_t size)
{
    FlushInstructionCache(GetCurrentProcess(), p, size);
}


void rxs::VirtualMemory::freeLargePagesMemory(void *p, size_t)
{
    VirtualFree(p, 0, MEM_RELEASE);
}


void rxs::VirtualMemory::osInit(size_t hugePageSize)
{
    if (hugePageSize) {
        m_hugePageSize = hugePageSize;
        return;
    }

    const size_t largePageMinimum = GetLargePageMinimum();
    if (largePageMinimum) {
        m_hugePageSize = largePageMinimum;
    }
}


bool rxs::VirtualMemory::allocateLargePagesMemory()
{
    m_scratchpad = static_cast<uint8_t *>(allocateLargePagesMemory(m_size));
    if (m_scratchpad) {
        m_flags.set(FLAG_HUGEPAGES, true);
        return true;
    }

    return false;
}


bool rxs::VirtualMemory::allocateOneGbPagesMemory()
{
    return false;
}


void rxs::VirtualMemory::freeLargePagesMemory()
{
    freeLargePagesMemory(m_scratchpad, m_size);
}
