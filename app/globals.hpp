/*
This file is part of ElectronPass.

ElectronPass is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ElectronPass is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ElectronPass. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include "data_holder.hpp"
#include "settings.hpp"
#include "sync/sync_manager.hpp"
#include "clipboard.hpp"
#include "wallet_merger.hpp"

class DataHolder;
class SyncManager;
class WalletMerger;

// Namespace in which are few global objects like settings and so on.
namespace globals {
    // Will be initialized later.
    extern DataHolder data_holder;
    extern SettingsManager settings;
    extern SyncManager sync_manager;
    extern Clipboard clipboard;
    extern WalletMerger wallet_merger;
}

#endif // GLOBALS_HPP
