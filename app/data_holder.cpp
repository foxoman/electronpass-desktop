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

#include "data_holder.hpp"

std::string DataHolder::read_file(bool& success) {
    std::string path = globals::settings.get_data_location().toStdString();
    std::string data;

    std::ifstream file(path);

    if (!file.good()) {
        success = false;
        return "";
    }
    // Because data is encoded in Base64, it is only in one line.
    std::getline(file, data);

    success = true;
    return data;
}

int DataHolder::unlock(const QString& password) {
    std::string password_string = password.toStdString();

    crypto = new electronpass::Crypto(password_string);
    if (!crypto->check()) return 1;

    bool success = false;
    std::string text = read_file(success);
    if (!success) return 2;

    text = crypto->decrypt(text, success);
    if (!success) return 3;

    wallet = electronpass::serialization::deserialize(text);

    for (auto item : wallet.get_items()) {
        item_names.push_back(QString::fromStdString(item.name));

        std::string subname = "";
        for (auto field : item.get_fields()) {
            if (!field.sensitive) {
                subname = field.value;
                break;
            }
        }
        item_subnames.push_back(QString::fromStdString(subname));
    }
    return 0;
}

void DataHolder::lock() {
    // Delete all decrypted data.
    delete[] crypto;
    wallet = electronpass::Wallet();
    item_names = {};
}

int DataHolder::get_number_of_items() {
    return item_names.size();
}

QString DataHolder::get_item_name(int id) {
    return item_names[id];
}

QString DataHolder::get_item_subname(int id) {
    return item_subnames[id];
}

QList<QMap<QString, QString>> DataHolder::get_item(int id) {
    QList<QMap<QString, QString>> fields;
    return fields;
}
