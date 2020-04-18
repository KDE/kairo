/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

*/

#include "timermodel.h"

TimerModel::TimerModel() = default;

TimerModel::TimerModel(const QString &text, int duration)
    : m_text{text},
      m_duration{duration}
{
}

bool TimerModel::operator==(const TimerModel &other) const
{
    return (m_text == other.m_text)
        && (m_duration == other.m_duration);
}

bool TimerModel::operator!=(const TimerModel &other) const
{
    return !(*this == other);
}

QString TimerModel::text() const
{
    return m_text;
}

int TimerModel::duration() const
{
    return m_duration;
}

TimerModel::Type TimerModel::type() const
{
    return m_duration < 0 ? Invalid
         : m_duration == 0 ? Stopwatch
         : Countdown;
}

bool TimerModel::isValid() const
{
    return type() != Invalid;
}
