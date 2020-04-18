/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

*/

import QtQuick 2.0
import Kairo 1.0

import org.kde.kirigami 2.0 as Kirigami


Kirigami.ApplicationWindow {
    id: root
    visible: true
    width: 640
    height: 480

    header: Kirigami.ApplicationHeader {}

    pageStack.initialPage: listComponent
    pageStack.defaultColumnWidth: width

    Connections {
        target: pageStack.contentItem
        onContentXChanged: {
            if (pageStack.depth > 1 && pageStack.contentItem.contentX === 0)
                pageStack.pop()
        }
    }

    SoundControl {
        id: soundControl
    }

    Component {
        id: listComponent
        CircuitListPage {
            onCircuitSelected: {
                var page = controlComponent.createObject(root)
                page.model = circuit
                page.soundControl = soundControl
                root.pageStack.push(page)
                page.start()
            }
        }
    }

    Component {
        id: controlComponent
        CircuitControlPage {
            onCircuitFinished: {
                root.pageStack.currentIndex = 0
                soundControl.playEndBeep()
            }
        }
    }
}
