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

#include "timernotificationcontrol.h"

#include <QtMath>

#include "soundcontrolinterface.h"
#include "timercontrol.h"

TimerNotificationControl::TimerNotificationControl(QObject *parent)
    : QObject{parent},
      m_sound{nullptr},
      m_timer{nullptr}
{
}

SoundControlInterface *TimerNotificationControl::sound() const
{
    return m_sound;
}

TimerControl *TimerNotificationControl::timer() const
{
    return m_timer;
}

void TimerNotificationControl::setSound(SoundControlInterface *sound)
{
    if (sound == m_sound)
        return;

    m_sound = sound;
    emit soundChanged(sound);
}

void TimerNotificationControl::setTimer(TimerControl *timer)
{
    if (timer == m_timer)
        return;

    if (m_timer) {
        disconnect(m_timer, &TimerControl::timerFinished, this, &TimerNotificationControl::onTimerFinished);
        disconnect(m_timer, &TimerControl::timerSkipped, this, &TimerNotificationControl::onTimerSkipped);
        disconnect(m_timer, &TimerControl::valueChanged, this, &TimerNotificationControl::onTimerValueChanged);
    }

    m_timer = timer;

    if (m_timer) {
        connect(m_timer, &TimerControl::timerFinished, this, &TimerNotificationControl::onTimerFinished);
        connect(m_timer, &TimerControl::timerSkipped, this, &TimerNotificationControl::onTimerSkipped);
        connect(m_timer, &TimerControl::valueChanged, this, &TimerNotificationControl::onTimerValueChanged);
    }

    emit timerChanged(timer);
}

void TimerNotificationControl::onTimerFinished()
{
    if (!m_sound)
        return;

    m_sound->playLongBeep();
}

void TimerNotificationControl::onTimerSkipped()
{
    if (!m_sound)
        return;

    m_sound->playShortBeep();
}

void TimerNotificationControl::onTimerValueChanged(int ms)
{
    if (!m_sound)
        return;

    if (m_timer->model().type() != TimerModel::Countdown)
        return;

    auto seconds = ms / 1000.0;
    if (static_cast<int>(seconds) == qCeil(seconds)
     && 1.0 <= seconds && seconds <= 10.0) {
        m_sound->playShortBeep();
    }
}
