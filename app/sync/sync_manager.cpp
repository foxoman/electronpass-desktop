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

#include "sync_manager.hpp"

#define kGdrive "gdrive"
#define kDropbox "dropbox"
#define kNone "none"

void SyncManager::setStatusMessage(const QString& message) {
    if (status_message != message) {
        status_message = message;
        emit statusMessageChanged();
    }
}

QString SyncManager::statusMessage() const {
    return status_message;
}

SyncManager::Service SyncManager::string_to_service(const std::string& string) {
    if (string == kGdrive) return SyncManager::Service::GDRIVE;
    if (string == kDropbox) return SyncManager::Service::DROPBOX;
    return SyncManager::Service::NONE;
}

std::string SyncManager::service_to_string(const Service& service) {
    if (service == SyncManager::Service::GDRIVE) return kGdrive;
    if (service == SyncManager::Service::DROPBOX) return kDropbox;
    return kNone;
}

SyncManager::SyncManager(QObject *parent): QObject(parent) {}

bool SyncManager::init() {
    if (initialized) {
        std::cout << "<sync_manager.cpp> [Warning] Sync manager already initialized. Ignoring init() call." << std::endl;
        return false;
    }
    initialized = true;

    Service service = string_to_service(globals::settings.sync_manager_get_service());
    set_service(service);

    return true;
}

void SyncManager::set_service(Service service) {
    globals::settings.sync_manager_set_service(service_to_string(service));
    if (sync_object != nullptr) delete sync_object;

    if (service == Service::NONE) {
        sync_object = nullptr;
        return;
    }

    if (service == Service::GDRIVE) sync_object = new Gdrive(this);
    if (service == Service::DROPBOX) sync_object = new Dropbox(this);

    connect(dynamic_cast<QObject*>(sync_object),
            SIGNAL(wallet_downloaded(const std::string&, SyncManagerStatus)),
            this,
            SLOT(service_did_download_wallet(const std::string&, SyncManagerStatus)));
    connect(dynamic_cast<QObject*>(sync_object),
            SIGNAL(wallet_uploaded(SyncManagerStatus)),
            this,
            SLOT(service_did_upload_wallet(SyncManagerStatus)));

    setStatusMessage("Sync manager initialized");
}

SyncManager::Service SyncManager::get_service() {
    return string_to_service(globals::settings.sync_manager_get_service());
}

void SyncManager::download_wallet(bool merge /* = true */) {
    need_to_merge = merge;
    if (!initialized) {
        std::cout << "<sync_manager.cpp> [Warning] Sync manager not initialized. Ignoring download_wallet() call." << std::endl;
    } else if (sync_object == nullptr) {
        emit wallet_downloaded("", static_cast<int>(SyncManagerStatus::NO_SYNC_PROVIDER));
    } else {
        setStatusMessage("Downloading wallet");
        sync_object->download_wallet();
    }
}

void SyncManager::upload_wallet() {
    if (!initialized) {
        std::cout << "<sync_manager.cpp> [Warning] Sync manager not initialized. Ignoring upload_wallet() call." << std::endl;
    } else if (sync_object == nullptr) {
        emit wallet_uploaded(static_cast<int>(SyncManagerStatus::NO_SYNC_PROVIDER));
    } else {
        setStatusMessage("Uploading wallet");

        bool success = false;
        std::string wallet = file_stream::read_file(success);
        if (!success) {
            emit wallet_uploaded(static_cast<int>(SyncManagerStatus::COULD_NOT_OPEN_FILE));
        } else {
            sync_object->upload_wallet(wallet);
        }
    }
}

void SyncManager::abort() {
    sync_object->abort();
}

void SyncManager::service_did_download_wallet(const std::string &wallet, SyncManagerStatus success) {
    if (success == SyncManagerStatus::SUCCESS) {
        if (need_to_merge) {
            setStatusMessage("Merging wallets");
            globals::wallet_merger.merge(wallet);
        } else {
            setStatusMessage("Saving wallet");
            file_stream::write_file(wallet);
        }
    }
    emit wallet_downloaded(QString(wallet.c_str()), static_cast<int>(success));
}

void SyncManager::service_did_upload_wallet(SyncManagerStatus success) {
    emit wallet_uploaded(static_cast<int>(success));
}
