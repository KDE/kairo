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

#include <QGuiApplication>
#include <QQuickView>

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

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:/main.qml"));
    view.show();

    return app.exec();
}
