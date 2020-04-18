/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

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
