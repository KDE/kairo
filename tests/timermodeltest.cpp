/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

*/

#include <QtTest>

#include "timermodel.h"

static_assert(std::is_copy_assignable<TimerModel>::value, "Should be copy assignable");
static_assert(std::is_copy_constructible<TimerModel>::value, "Should be copy constructible");
static_assert(std::is_move_assignable<TimerModel>::value, "Should be move assignable");
static_assert(std::is_move_constructible<TimerModel>::value, "Should be move constructible");

class TimerModelTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    {
        qRegisterMetaType<TimerModel>();
    }

    void shouldHaveDefaultState()
    {
        // GIVEN
        auto model = TimerModel{};

        // THEN
        QCOMPARE(model.text(), QString());
        QCOMPARE(model.duration(), -1);
        QCOMPARE(model.type(), TimerModel::Invalid);
        QVERIFY(!model.isValid());
    }

    void shouldConstructCountdown()
    {
        // GIVEN
        auto model = TimerModel{"rest", 10000};

        // THEN
        QCOMPARE(model.text(), QString("rest"));
        QCOMPARE(model.duration(), 10000);
        QCOMPARE(model.type(), TimerModel::Countdown);
        QVERIFY(model.isValid());
    }

    void shouldConstructStopwatch()
    {
        // GIVEN
        auto model = TimerModel{"run"};

        // THEN
        QCOMPARE(model.text(), QString("run"));
        QCOMPARE(model.duration(), 0);
        QCOMPARE(model.type(), TimerModel::Stopwatch);
        QVERIFY(model.isValid());
    }

    void shouldBeCopyable()
    {
        // GIVEN
        auto model = TimerModel{"rest", 10000};

        // WHEN
        model = TimerModel{"foo", 5000};

        // THEN
        QCOMPARE(model.text(), QString("foo"));
        QCOMPARE(model.duration(), 5000);
    }

    void shouldBeComparable_data()
    {
        QTest::addColumn<TimerModel>("model1");
        QTest::addColumn<TimerModel>("model2");
        QTest::addColumn<bool>("expected");

        QTest::newRow("equal") << TimerModel{"foo", 5000} << TimerModel{"foo", 5000} << true;
        QTest::newRow("differ by text") << TimerModel{"foo", 5000} << TimerModel{"bar", 5000} << false;
        QTest::newRow("differ by duration") << TimerModel{"foo", 5000} << TimerModel{"foo", 15000} << false;
        QTest::newRow("differ completely") << TimerModel{"foo", 5000} << TimerModel{"bar", 15000} << false;
    }

    void shouldBeComparable()
    {
        // GIVEN
        QFETCH(TimerModel, model1);
        QFETCH(TimerModel, model2);
        QFETCH(bool, expected);

        // THEN
        QCOMPARE((model1 == model2), expected);
        QCOMPARE((model1 != model2), !expected);
    }
};

QTEST_APPLESS_MAIN(TimerModelTest)

#include "timermodeltest.moc"
