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

#include "timercontrol.h"

#include <QTimer>

TimerControl::TimerControl(QObject *parent)
    : QObject{parent},
      m_timer{new QTimer{this}}
{
    m_timer->setTimerType(Qt::PreciseTimer);
    m_timer->setInterval(10);
    connect(m_timer, &QTimer::timeout, this, &TimerControl::onTimerTimeout);
}

TimerModel TimerControl::createModel() const
{
    return {};
}

TimerModel TimerControl::createModel(const QString &text, int duration) const
{
    return {text, duration};
}

TimerModel TimerControl::model() const
{
    return m_model;
}

void TimerControl::setModel(const TimerModel &model)
{
    if (model == m_model)
        return;

    auto m = model;
    setModel(std::move(m));
}

void TimerControl::setModel(TimerModel &&model)
{
    if (model == m_model)
        return;

    const auto oldText = text();
    const auto oldValue = value();
    const auto oldDuration = duration();
    const auto skipped = m_running && m_model.isValid() && oldValue != 0;

    m_model = std::move(model);

    m_value = m_model.type() == TimerModel::Countdown ? m_model.duration() : 0;

    if (m_running)
         m_timer->start();

    emit modelChanged(m_model);

    if (skipped)
        emit timerSkipped();

    if (oldText != text())
        emit textChanged(text());

    if (oldValue != value())
        emit valueChanged(value());

    if (oldDuration != duration())
        emit durationChanged(duration());
}

QString TimerControl::text() const
{
    return m_model.text();
}

int TimerControl::value() const
{
    return m_value;
}

int TimerControl::duration() const
{
    return m_model.duration();
}

bool TimerControl::isRunning() const
{
    return m_running;
}

void TimerControl::setRunning(bool running)
{
    if (running == m_running)
        return;

    m_running = running;
    emit runningChanged(m_running);

    if (!m_model.isValid()) {
        if (m_running)
            emit timerFinished();
        m_timer->stop();
    } else if (!m_running) {
        m_timer->stop();
    } else {
        m_timer->start();
    }
}

void TimerControl::onTimerTimeout()
{
    m_value += m_model.type() == TimerModel::Countdown ? -10 : 10;
    emit valueChanged(m_value);

    if (m_value == 0) {
        m_timer->stop();
        emit timerFinished();
    }
}
