/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

*/

import QtQuick 2.0
import QtQuick.Controls 2.15 as Controls
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
        Controls.Label {
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter

            font.pixelSize: Kirigami.Theme.defaultFont.pixelSize * 6
            text: timer.formattedValue
        }
    }
}
