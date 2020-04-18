/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

*/

#ifndef TIMERNOTIFICATIONCONTROL_H
#define TIMERNOTIFICATIONCONTROL_H

#include <QObject>

class SoundControlInterface;
class TimerControl;

class TimerNotificationControl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SoundControlInterface* sound READ sound WRITE setSound NOTIFY soundChanged)
    Q_PROPERTY(TimerControl* timer READ timer WRITE setTimer NOTIFY timerChanged)
public:
    explicit TimerNotificationControl(QObject *parent = nullptr);

    SoundControlInterface *sound() const;
    TimerControl *timer() const;

public slots:
    void setSound(SoundControlInterface *sound);
    void setTimer(TimerControl *timer);

signals:
    void soundChanged(SoundControlInterface *timer);
    void timerChanged(TimerControl *timer);

private slots:
    void onTimerFinished();
    void onTimerSkipped();
    void onTimerValueChanged(int ms);

private:
    SoundControlInterface *m_sound;
    TimerControl *m_timer;
};

#endif
