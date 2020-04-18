/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

*/

#include "timercontrol.h"

#include <QTimer>
#include <QTime>
#include <QtCore/qmath.h>

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

    if (oldValue != value()) {
        emit valueChanged(value());
        emit formattedValueChanged(formattedValue());
    }

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

QString TimerControl::formattedValue() const
{
    qreal time = qRound((qreal)m_value / (qreal)100)/(qreal)10;
    int minutes = qFloor(time / 60);
    //seconds rounded to first decimal position
    int seconds = qFloor(time - minutes * 60);

    return QStringLiteral("%1:%2.%3")
        .arg(minutes, 2, 10, QLatin1Char('0'))
        .arg(seconds, 2, 10, QLatin1Char('0'))
        .arg(((time - qFloor(time)) * 10));
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
    emit formattedValueChanged(formattedValue());

    if (m_value == 0) {
        m_timer->stop();
        emit timerFinished();
    }
}
