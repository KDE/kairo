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

#ifndef CIRCUITCONTROL_H
#define CIRCUITCONTROL_H

#include <QObject>

#include "circuitmodel.h"

class TimerControl;

class CircuitControl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(CircuitModel model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(TimerControl* timerControl READ timerControl WRITE setTimerControl NOTIFY timerControlChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(int size READ size NOTIFY sizeChanged)
    Q_PROPERTY(int remaining READ remaining NOTIFY remainingChanged)
    Q_PROPERTY(TimerModel currentTimer READ currentTimer NOTIFY currentTimerChanged)
public:
    explicit CircuitControl(QObject *parent = nullptr);

    CircuitModel model() const;
    void setModel(const CircuitModel &model);
    void setModel(CircuitModel &&model);

    TimerControl *timerControl() const;
    void setTimerControl(TimerControl *timerControl);

    QString name() const;
    int size() const;
    int remaining() const;
    TimerModel currentTimer() const;

public slots:
    void nextTimer();

signals:
    void modelChanged(const CircuitModel &model);
    void timerControlChanged(TimerControl *timerControl);
    void nameChanged(const QString &name);
    void sizeChanged(int size);
    void remainingChanged(int remaining);
    void currentTimerChanged(const TimerModel &model);

    void circuitFinished();

private:
    void applyCurrentTimer();

    CircuitModel m_model;
    int m_currentIndex;
    TimerControl *m_timerControl;
};

#endif
