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

#include "circuitcontrol.h"

#include "timercontrol.h"

CircuitControl::CircuitControl(QObject *parent)
    : QObject{parent},
      m_currentIndex{0},
      m_timerControl{nullptr}
{
}

CircuitModel CircuitControl::model() const
{
    return m_model;
}

void CircuitControl::setModel(const CircuitModel &model)
{
    if (model == m_model)
        return;

    auto m = model;
    setModel(std::move(m));
}

void CircuitControl::setModel(CircuitModel &&model)
{
    if (model == m_model)
        return;

    const auto oldName = name();
    const auto oldSize = size();
    const auto oldRemaining = remaining();

    m_model = std::move(model);

    emit modelChanged(m_model);

    if (oldName != name())
        emit nameChanged(name());

    if (oldSize != size())
        emit sizeChanged(size());

    if (oldRemaining != remaining())
        emit remainingChanged(size());


    m_currentIndex = 0;
    applyCurrentTimer();
    emit currentTimerChanged(currentTimer());
}

TimerControl *CircuitControl::timerControl() const
{
    return m_timerControl;
}

void CircuitControl::setTimerControl(TimerControl *timerControl)
{
    if (timerControl == m_timerControl)
        return;

    if (m_timerControl) {
        connect(m_timerControl, &TimerControl::timerFinished, this, &CircuitControl::nextTimer);
    }

    m_timerControl = timerControl;

    if (m_timerControl) {
        connect(m_timerControl, &TimerControl::timerFinished, this, &CircuitControl::nextTimer);
    }

    emit timerControlChanged(timerControl);

    applyCurrentTimer();
}

QString CircuitControl::name() const
{
    return m_model.name();
}

int CircuitControl::size() const
{
    return m_model.size();
}

int CircuitControl::remaining() const
{
    if (currentTimer().isValid())
        return m_model.size() - m_currentIndex;
    else
        return 0;
}

TimerModel CircuitControl::currentTimer() const
{
    if (m_currentIndex < m_model.size())
        return m_model.at(m_currentIndex);
    else
        return {};
}

void CircuitControl::nextTimer()
{
    const auto oldRemaining = remaining();

    m_currentIndex++;
    applyCurrentTimer();
    emit currentTimerChanged(currentTimer());

    if (oldRemaining != remaining()) {
        emit remainingChanged(remaining());

        if (remaining() == 0)
            emit circuitFinished();
    }
}

void CircuitControl::applyCurrentTimer()
{
    if (m_timerControl && m_timerControl->model() != currentTimer())
        m_timerControl->setModel(currentTimer());
}
