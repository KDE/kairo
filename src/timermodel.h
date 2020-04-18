/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

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
