/* XMRig
 * Copyright (c) 2016-2024 XMRig       <https://github.com/xmrig>, <support@xmrig.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "App.h"
#include "base/io/log/Log.h"
#include "core/Controller.h"


bool rxs::App::background(int &)
{
    if (m_controller->isBackground()) {
        LOG_WARN("background mode is not supported on Windows; staying in the foreground");
    }

    return false;
}
