/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

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
