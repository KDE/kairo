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
