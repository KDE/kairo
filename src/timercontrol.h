/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

*/

#ifndef TIMERCONTROL_H
#define TIMERCONTROL_H

#include <QObject>

#include "timermodel.h"

class QTimer;

class TimerControl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(TimerModel model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QString text READ text NOTIFY textChanged)
    Q_PROPERTY(int value READ value NOTIFY valueChanged)
    Q_PROPERTY(QString formattedValue READ formattedValue NOTIFY formattedValueChanged)
    Q_PROPERTY(int duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(bool running READ isRunning WRITE setRunning NOTIFY runningChanged)
public:
    explicit TimerControl(QObject *parent = nullptr);

    Q_INVOKABLE TimerModel createModel() const;
    Q_INVOKABLE TimerModel createModel(const QString &text, int duration = 0) const;

    TimerModel model() const;
    void setModel(const TimerModel &model);
    void setModel(TimerModel &&model);

    QString text() const;
    int value() const;
    QString formattedValue() const;
    int duration() const;
    bool isRunning() const;

public slots:
    void setRunning(bool running);

signals:
    void modelChanged(const TimerModel &model);
    void textChanged(const QString &text);
    void valueChanged(int value);
    void durationChanged(int duration);
    void runningChanged(bool running);
    void formattedValueChanged(const QString &formattedValue);

    void timerFinished();
    void timerSkipped();

private slots:
    void onTimerTimeout();

private:
    TimerModel m_model;
    QTimer *m_timer;
    bool m_running = false;
    int m_value = 0;
};

#endif
