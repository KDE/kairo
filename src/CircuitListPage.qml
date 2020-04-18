/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

*/

import QtQuick 2.0
import org.kde.kirigami 2.0 as Kirigami
import Kairo 1.0

Kirigami.ScrollablePage {
    id: root

    title: "Available Circuits"
    background: Rectangle {
        color: Kirigami.Theme.viewBackgroundColor
    }

    signal circuitSelected(var circuit)

    CircuitListModel {
        id: listModel
    }

    Text {
        anchors.centerIn: parent
        width: parent.width - 32
        visible: listView.count === 0
        font.pixelSize: Kirigami.Theme.defaultFont.pixelSize * 2
        wrapMode: Text.Wrap
        text: "Place some circuit files in " + listModel.path
    }

    ListView {
        id: listView
        anchors.fill: parent
        model: listModel
        delegate: Kirigami.BasicListItem {
            reserveSpaceForIcon: false
            label: model.display
            onClicked: root.circuitSelected(listModel.loadCircuit(model.index))
        }
    }
}
