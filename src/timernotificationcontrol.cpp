/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

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
