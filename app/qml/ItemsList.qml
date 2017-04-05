import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Material 2.1

ListView {
    id: listView
    anchors.left: parent.left
    anchors.right: viewDivider.left
    anchors.bottom: parent.bottom
    anchors.top: parent.top

    ScrollIndicator.vertical: ScrollIndicator {
        id: scrollIndicator
        active: true

        anchors.bottom: parent.bottom
        anchors.top: parent.top
    }

    model: 20

    delegate: ItemDelegate {
        text: modelData
        width: listView.width - scrollIndicator.width
    }
}
