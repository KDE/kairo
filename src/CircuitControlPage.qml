/* This file is part of Kairo Timer

   Copyright 2016 Kevin Ottens <ervin@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License or (at your option) version 3 or any later version
   accepted by the membership of KDE e.V. (or its successor approved
   by the membership of KDE e.V.), which shall act as a proxy
   defined in Section 14 of version 3 of the license.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
   USA.
*/

import QtQuick 2.0
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.0 as Kirigami
import Kairo 1.0

Kirigami.Page {
    id: root

    title: model.name
    topPadding: 0
    leftPadding: 0
    rightPadding: 0
    bottomPadding: 0

    property SoundControl soundControl
    property alias model: circuitControl.model
    signal circuitFinished()

    function start()
    {
        timer.running = true
    }

    actions.main: Kirigami.Action {
        iconName: "go-next"
        onTriggered: circuitControl.nextTimer()
    }

    TimerNotificationControl {
        sound: root.soundControl
        timer: TimerControl {
            id: timer
            running: false
        }
    }

    CircuitControl {
        id: circuitControl
        timerControl: timer
        onCircuitFinished: {
            timer.running = false
            root.circuitFinished()
        }
    }

    Rectangle {
        height: parent.height
        width: timer.duration <= 0 ? 0 : parent.width * (timer.duration - timer.value) / timer.duration
        color: timer.value > 10000 ? "lightSteelBlue" : "crimson"
    }

    ColumnLayout {
        anchors {
            left: parent.left
            right: parent.right
            verticalCenter: parent.verticalCenter
        }
        spacing: Kirigami.Units.gridUnit
        Kirigami.Heading {
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: Kirigami.Theme.defaultFont.pixelSize * 4
            text: timer.text
            wrapMode: Text.WordWrap
        }
        Kirigami.Label {
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter

            font.pixelSize: Kirigami.Theme.defaultFont.pixelSize * 6
            text: timer.formattedValue
        }
    }
}
