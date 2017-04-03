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

import QtQuick 2.2
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.1
import Qt.labs.settings 1.0

Dialog {
    title: qsTr("Settings")

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    modal: true

    width: Math.min(parent.width * 0.7, 600)
    height: Math.min(parent.height * 0.7, 400)

    ColumnLayout {
        id: settingsList

        Switch {
            id: themeSwitch
            text: qsTr("Dark theme (restart application to apply)")
            checked: settings.theme
        }

        width: parent.width
    }

    Component.onDestruction: {
        settings.theme = themeSwitch.checked
    }

}
