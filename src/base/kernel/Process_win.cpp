/* XMRig
 * Copyright (c) 2016-2024 XMRig       <https://github.com/xmrig>, <support@xmrig.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <windows.h>
#include "base/kernel/Process.h"


int rxs::Process::pid()
{
    return static_cast<int>(GetCurrentProcessId());
}
