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

#ifndef TIMERMODEL_H
#define TIMERMODEL_H

#include <QMetaType>
#include <QString>

class TimerModel
{
    Q_GADGET
    Q_PROPERTY(QString text READ text)
    Q_PROPERTY(int duration READ duration)
    Q_PROPERTY(Type type READ type)
    Q_PROPERTY(bool valid READ isValid)
public:
    enum Type {
        Invalid,
        Countdown,
        Stopwatch
    };
    Q_ENUM(Type)

    TimerModel();
    TimerModel(const QString &text, int duration = 0);

    bool operator==(const TimerModel &other) const;
    bool operator!=(const TimerModel &other) const;

    QString text() const;
    int duration() const;
    Type type() const;

    bool isValid() const;

private:
    QString m_text = {};
    int m_duration = -1;
};

Q_DECLARE_METATYPE(TimerModel)

#endif
