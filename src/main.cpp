/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

*/

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "circuitcontrol.h"
#include "circuitlistmodel.h"
#include "circuitmodel.h"
#include "timercontrol.h"
#include "timermodel.h"
#include "timernotificationcontrol.h"
#include "soundcontrol.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    app.setApplicationName("Kairo");

    qmlRegisterType<CircuitListModel>("Kairo", 1, 0, "CircuitListModel");
    qmlRegisterUncreatableType<CircuitModel>("Kairo", 1, 0, "CircuitModel", "Loaded from C++");
    qmlRegisterType<CircuitControl>("Kairo", 1, 0, "CircuitControl");
    qmlRegisterUncreatableType<TimerModel>("Kairo", 1, 0, "TimerModel", "Use TimerControl.createModel()");
    qmlRegisterType<TimerControl>("Kairo", 1, 0, "TimerControl");
    qmlRegisterType<TimerNotificationControl>("Kairo", 1, 0, "TimerNotificationControl");
    qmlRegisterUncreatableType<SoundControlInterface>("Kairo", 1, 0, "SoundControlInterface", "Use SoundControl");
    qmlRegisterType<SoundControl>("Kairo", 1, 0, "SoundControl");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
