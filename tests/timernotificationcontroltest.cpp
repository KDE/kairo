/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

*/

#include <QtTest>
#include <QSignalSpy>

#include <memory>

#include "timernotificationcontrol.h"

#include "timercontrol.h"
#include "soundcontrolstub.h"

class TimerNotificationControlTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    {
        qRegisterMetaType<TimerControl*>();
        qRegisterMetaType<SoundControlInterface*>();
    }

    void shouldHaveDefaultState()
    {
        // GIVEN
        auto notification = std::make_unique<TimerNotificationControl>();

        // THEN
        QCOMPARE(notification->sound(), static_cast<SoundControlInterface*>(nullptr));
        QCOMPARE(notification->timer(), static_cast<TimerControl*>(nullptr));
    }

    void shouldHaveSoundControl()
    {
        // GIVEN
        auto notification = std::make_unique<TimerNotificationControl>();
        auto sound = new SoundControlStub(notification.get());
        auto soundSpy = std::make_unique<QSignalSpy>(notification.get(), &TimerNotificationControl::soundChanged);

        // WHEN
        notification->setSound(sound);

        // THEN
        QCOMPARE(notification->sound(), sound);
        QCOMPARE(soundSpy->size(), 1);
        QCOMPARE(soundSpy->takeFirst().first().value<SoundControlInterface*>(), sound);

        // WHEN
        notification->setSound(sound);

        // THEN
        QCOMPARE(soundSpy->size(), 0);
    }

    void shouldHaveTimerControl()
    {
        // GIVEN
        auto notification = std::make_unique<TimerNotificationControl>();
        auto timer = new TimerControl(notification.get());
        auto timerSpy = std::make_unique<QSignalSpy>(notification.get(), &TimerNotificationControl::timerChanged);

        // WHEN
        notification->setTimer(timer);

        // THEN
        QCOMPARE(notification->timer(), timer);
        QCOMPARE(timerSpy->size(), 1);
        QCOMPARE(timerSpy->takeFirst().first().value<TimerControl*>(), timer);

        // WHEN
        notification->setTimer(timer);

        // THEN
        QCOMPARE(timerSpy->size(), 0);
    }

    void shouldPlayLongBeepOnTimerFinished()
    {
        // GIVEN
        auto notification = std::make_unique<TimerNotificationControl>();

        auto sound = new SoundControlStub(notification.get());
        notification->setSound(sound);

        auto timer = new TimerControl(notification.get());
        notification->setTimer(timer);

        // WHEN
        emit timer->timerFinished();

        // THEN
        QCOMPARE(sound->calls().size(), 1);
        QCOMPARE(sound->calls().first(), SoundControlStub::LongBeep);
        sound->clear();

        // WHEN
        notification->setTimer(nullptr);
        emit timer->timerFinished();

        // THEN
        QVERIFY(sound->calls().isEmpty());
    }

    void shouldPlayShortBeepOnTimerSkipped()
    {
        // GIVEN
        auto notification = std::make_unique<TimerNotificationControl>();

        auto sound = new SoundControlStub(notification.get());
        notification->setSound(sound);

        auto timer = new TimerControl(notification.get());
        notification->setTimer(timer);

        // WHEN
        emit timer->timerSkipped();

        // THEN
        QCOMPARE(sound->calls().size(), 1);
        QCOMPARE(sound->calls().first(), SoundControlStub::ShortBeep);
        sound->clear();

        // WHEN
        notification->setTimer(nullptr);
        emit timer->timerSkipped();

        // THEN
        QVERIFY(sound->calls().isEmpty());
    }

    void shouldPlayShortBeepOnLastTenCountdownPlainSecondsOnly()
    {
        // GIVEN
        auto notification = std::make_unique<TimerNotificationControl>();

        auto sound = new SoundControlStub(notification.get());
        notification->setSound(sound);

        auto timer = new TimerControl(notification.get());
        timer->setModel({"countdown", 50000});
        notification->setTimer(timer);

        // WHEN
        emit timer->valueChanged(50000);
        emit timer->valueChanged(47550);
        emit timer->valueChanged(10100);
        emit timer->valueChanged(10010);
        emit timer->valueChanged(10000);
        emit timer->valueChanged(9990);
        emit timer->valueChanged(9010);
        emit timer->valueChanged(9000);
        emit timer->valueChanged(8990);
        emit timer->valueChanged(8000);
        emit timer->valueChanged(7000);
        emit timer->valueChanged(6000);
        emit timer->valueChanged(5000);
        emit timer->valueChanged(4000);
        emit timer->valueChanged(3000);
        emit timer->valueChanged(2000);
        emit timer->valueChanged(1000);
        emit timer->valueChanged(0);

        // THEN
        QCOMPARE(sound->calls().size(), 10);
        QCOMPARE(sound->calls().first(), SoundControlStub::ShortBeep);
        sound->clear();

        // WHEN
        timer->setModel({"timer"});
        emit timer->valueChanged(9000);

        // THEN
        QVERIFY(sound->calls().isEmpty());

        // WHEN
        notification->setTimer(nullptr);
        emit timer->valueChanged(9000);

        // THEN
        QVERIFY(sound->calls().isEmpty());
    }
};

QTEST_MAIN(TimerNotificationControlTest)

#include "timernotificationcontroltest.moc"
