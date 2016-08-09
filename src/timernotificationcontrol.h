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
