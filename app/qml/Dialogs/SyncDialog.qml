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
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1

Dialog {
    id: syncDialog
    modal: true

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    closePolicy: Popup.NoAutoClose

    function sync() {
        syncManager.download_wallet();
    }

    Connections {
        target: syncManager

        onStatusMessageChanged: {
            statusLabel.text = syncManager.statusMessage
        }
        onWallet_downloaded: {
            console.log("wallet downloaded");
            syncDialog.close();
        }
    }


    ColumnLayout {
        anchors.fill: parent
        BusyIndicator {
            Layout.alignment: Qt.AlignHCenter
            running: true
        }
        Label {
            id: statusLabel
            Layout.alignment: Qt.AlignHCenter
            text: syncManager.statusMessage
        }
    }

}
