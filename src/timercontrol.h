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
    bool isRunning() const;

public slots:
    void setRunning(bool running);

signals:
    void modelChanged(const TimerModel &model);
    void textChanged(const QString &text);
    void valueChanged(int value);
    void runningChanged(bool running);

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
