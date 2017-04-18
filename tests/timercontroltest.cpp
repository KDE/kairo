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
#include <QSignalSpy>

#include <memory>

#include "timercontrol.h"

class TimerControlTest : public QObject
{
    Q_OBJECT
private slots:
    void shouldHaveDefaultState()
    {
        // GIVEN
        auto control = std::make_unique<TimerControl>();

        // THEN
        QCOMPARE(control->model(), TimerModel{});
        QCOMPARE(control->text(), QString());
        QCOMPARE(control->value(), 0);
        QCOMPARE(control->formattedValue(), QStringLiteral("00:00:00.0"));
        QCOMPARE(control->duration(), -1);
        QCOMPARE(control->isRunning(), false);
    }

    void shouldDeriveValuesFromModel_data()
    {
        QTest::addColumn<TimerModel>("model");
        QTest::addColumn<QString>("expectedText");
        QTest::addColumn<int>("expectedValue");
        QTest::addColumn<QString>("expectedFormattedValue");
        QTest::addColumn<int>("expectedDuration");

        QTest::newRow("invalid") << TimerModel{} << QString() << 0 << QStringLiteral("00:00:00.0") <<-1;
        QTest::newRow("countdown") << TimerModel{"foo", 10000} << "foo" << 10000 << QStringLiteral("00:00:10.0") << 10000;
        QTest::newRow("countdown") << TimerModel{"baz", 1050300} << "baz" << 1050300 << QStringLiteral("00:17:30.3") << 1050300;
        QTest::newRow("countdown") << TimerModel{"baz", 34570900} << "baz" << 34570900 << QStringLiteral("09:36:10.9") << 34570900;
        QTest::newRow("stopwatch") << TimerModel{"bar"} << "bar" << 0 << QStringLiteral("00:00:00.0") << 0;
    }

    void shouldDeriveValuesFromModel()
    {
        // GIVEN
        auto control = std::make_unique<TimerControl>();
        auto modelSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::modelChanged);
        auto textSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::textChanged);
        auto valueSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::valueChanged);
        auto durationSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::durationChanged);

        QFETCH(TimerModel, model);
        QFETCH(QString, expectedText);
        QFETCH(int, expectedValue);
        QFETCH(QString, expectedFormattedValue);
        QFETCH(int, expectedDuration);

        // WHEN
        control->setModel(model);

        // THEN
        QCOMPARE(control->model(), model);
        QCOMPARE(control->text(), expectedText);
        QCOMPARE(control->value(), expectedValue);
        QCOMPARE(control->formattedValue(), expectedFormattedValue);
        QCOMPARE(control->duration(), expectedDuration);

        if (model.isValid()) {
            QCOMPARE(modelSpy->size(), 1);
            QCOMPARE(modelSpy->takeFirst().first().value<TimerModel>(), model);

            QCOMPARE(textSpy->size(), 1);
            QCOMPARE(textSpy->takeFirst().first().toString(), expectedText);

            if (model.duration() >= 0) {
                QCOMPARE(durationSpy->size(), 1);
                QCOMPARE(durationSpy->takeFirst().first().toInt(), expectedDuration);
            } else {
                QCOMPARE(durationSpy->size(), 0);
            }

            if (model.duration() != 0) {
                QCOMPARE(valueSpy->size(), 1);
                QCOMPARE(valueSpy->takeFirst().first().toInt(), expectedValue);
            } else {
                QCOMPARE(valueSpy->size(), 0);
            }
        } else {
            QCOMPARE(textSpy->size(), 0);
            QCOMPARE(valueSpy->size(), 0);
            QCOMPARE(durationSpy->size(), 0);
        }

        // WHEN
        control->setModel(model);

        // THEN
        QCOMPARE(textSpy->size(), 0);
        QCOMPARE(valueSpy->size(), 0);
        QCOMPARE(durationSpy->size(), 0);
    }

    void shouldDeriveValuesFromMovedModel_data()
    {
        shouldDeriveValuesFromModel_data();
    }

    void shouldDeriveValuesFromMovedModel()
    {
        // GIVEN
        auto control = std::make_unique<TimerControl>();
        auto textSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::textChanged);
        auto valueSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::valueChanged);
        auto durationSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::durationChanged);

        QFETCH(TimerModel, model);
        QFETCH(QString, expectedText);
        QFETCH(int, expectedValue);
        QFETCH(QString, expectedFormattedValue);
        QFETCH(int, expectedDuration);

        // WHEN
        auto copy = model;
        control->setModel(std::move(copy)); // This should move it in place

        // THEN
        QCOMPARE(control->model(), model);
        QCOMPARE(control->text(), expectedText);
        QCOMPARE(control->value(), expectedValue);
        QCOMPARE(control->formattedValue(), expectedFormattedValue);
        QCOMPARE(control->duration(), expectedDuration);

        if (model.isValid()) {
            QCOMPARE(textSpy->size(), 1);
            QCOMPARE(textSpy->takeFirst().first().toString(), expectedText);

            if (model.duration() >= 0) {
                QCOMPARE(durationSpy->size(), 1);
                QCOMPARE(durationSpy->takeFirst().first().toInt(), expectedDuration);
            } else {
                QCOMPARE(durationSpy->size(), 0);
            }

            if (model.duration() != 0) {
                QCOMPARE(valueSpy->size(), 1);
                QCOMPARE(valueSpy->takeFirst().first().toInt(), expectedValue);
            } else {
                QCOMPARE(valueSpy->size(), 0);
            }
        } else {
            QCOMPARE(textSpy->size(), 0);
            QCOMPARE(valueSpy->size(), 0);
        }

        // WHEN
        control->setModel(std::move(model));

        // THEN
        QCOMPARE(textSpy->size(), 0);
        QCOMPARE(valueSpy->size(), 0);
        QCOMPARE(durationSpy->size(), 0);
    }

    void shouldDoNothingWhileRunningInvalid()
    {
        // GIVEN
        auto control = std::make_unique<TimerControl>();
        auto textSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::textChanged);
        auto valueSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::valueChanged);
        auto durationSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::durationChanged);
        auto runningSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::runningChanged);
        auto finishedSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::timerFinished);
        control->setModel(TimerModel{});

        // WHEN
        control->setRunning(true);
        QTest::qWait(100);

        // THEN
        QCOMPARE(control->isRunning(), true);
        QCOMPARE(textSpy->size(), 0);
        QCOMPARE(valueSpy->size(), 0);
        QCOMPARE(durationSpy->size(), 0);
        QCOMPARE(runningSpy->size(), 1);
        QCOMPARE(runningSpy->takeFirst().first().toBool(), true);
        QCOMPARE(finishedSpy->size(), 1);
    }

    void shouldRunCountdown()
    {
        // GIVEN
        auto control = std::make_unique<TimerControl>();
        auto textSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::textChanged);
        auto valueSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::valueChanged);
        auto durationSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::durationChanged);
        auto runningSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::runningChanged);
        auto finishedSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::timerFinished);

        control->setModel(TimerModel{"rest", 100});
        textSpy->clear();
        valueSpy->clear();
        durationSpy->clear();

        // WHEN
        control->setRunning(true);
        QTest::qWait(105);

        // THEN
        QCOMPARE(control->isRunning(), true);
        QCOMPARE(textSpy->size(), 0);
        QCOMPARE(durationSpy->size(), 0);
        QCOMPARE(valueSpy->size(), 10);
        for (int value = 100; value != 0; value -= 10) {
            QCOMPARE(valueSpy->takeFirst().first().toInt(), value - 10);
        }
        QCOMPARE(runningSpy->size(), 1);
        QCOMPARE(runningSpy->takeFirst().first().toBool(), true);
        QCOMPARE(finishedSpy->size(), 1);
    }

    void shouldRunStopwatch()
    {
        // GIVEN
        auto control = std::make_unique<TimerControl>();
        auto textSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::textChanged);
        auto valueSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::valueChanged);
        auto durationSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::durationChanged);
        auto runningSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::runningChanged);
        auto finishedSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::timerFinished);

        control->setModel(TimerModel{"run"});
        textSpy->clear();
        valueSpy->clear();
        durationSpy->clear();

        // WHEN
        control->setRunning(true);
        QTest::qWait(105);
        control->setRunning(false);

        // THEN
        QCOMPARE(control->isRunning(), false);
        QCOMPARE(textSpy->size(), 0);
        QCOMPARE(durationSpy->size(), 0);
        QCOMPARE(valueSpy->size(), 10);
        for (int value = 0; value != 100; value += 10) {
            QCOMPARE(valueSpy->takeFirst().first().toInt(), value + 10);
        }
        QCOMPARE(runningSpy->size(), 2);
        QCOMPARE(runningSpy->takeFirst().first().toBool(), true);
        QCOMPARE(runningSpy->takeFirst().first().toBool(), false);
        QCOMPARE(finishedSpy->size(), 0);
    }

    void shouldKeepRunningDuringModelChanges()
    {
        // GIVEN
        auto control = std::make_unique<TimerControl>();
        auto textSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::textChanged);
        auto valueSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::valueChanged);
        auto durationSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::durationChanged);
        auto runningSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::runningChanged);
        auto finishedSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::timerFinished);
        auto skippedSpy = std::make_unique<QSignalSpy>(control.get(), &TimerControl::timerSkipped);

        control->setModel(TimerModel{"run"});
        textSpy->clear();
        valueSpy->clear();
        durationSpy->clear();

        // WHEN
        control->setRunning(true);
        QTest::qWait(25);
        control->setModel(TimerModel{"rest", 100});
        QTest::qWait(105);
        control->setModel(TimerModel{"run"});
        QTest::qWait(35);
        control->setModel(TimerModel{"rest again", 100});
        QTest::qWait(55);
        control->setRunning(false);

        // THEN
        QCOMPARE(control->isRunning(), false);
        QCOMPARE(runningSpy->size(), 2);
        QCOMPARE(runningSpy->takeFirst().first().toBool(), true);
        QCOMPARE(runningSpy->takeFirst().first().toBool(), false);

        QCOMPARE(textSpy->size(), 3);
        QCOMPARE(textSpy->takeFirst().first().toString(), QString("rest"));
        QCOMPARE(textSpy->takeFirst().first().toString(), QString("run"));
        QCOMPARE(textSpy->takeFirst().first().toString(), QString("rest again"));

        const auto expectedValues = QVector<int>{10, 20, 100, 90, 80, 70, 60, 50, 40, 30, 20, 10, 0, 10, 20, 30, 100, 90, 80, 70, 60, 50};
        auto values = QVector<int>();
        while (!valueSpy->isEmpty())
            values << valueSpy->takeFirst().first().toInt();
        qDebug() << values;
        qDebug() << expectedValues;
        QCOMPARE(values, expectedValues);

        const auto expectedDurations = QVector<int>{100, 0, 100};
        auto durations = QVector<int>();
        while (!durationSpy->isEmpty())
            durations << durationSpy->takeFirst().first().toInt();
        qDebug() << durations;
        qDebug() << expectedDurations;
        QCOMPARE(values, expectedValues);

        QCOMPARE(finishedSpy->size(), 1);
        QCOMPARE(skippedSpy->size(), 2);
    }

    void shouldCreateModels()
    {
        // GIVEN
        auto control = std::make_unique<TimerControl>();

        // THEN
        auto model = TimerModel{};
        QCOMPARE(control->createModel(), model);

        model = TimerModel{"foo"};
        QCOMPARE(control->createModel("foo"), model);

        model = TimerModel{"bar", 1000};
        QCOMPARE(control->createModel("bar", 1000), model);
    }
};

QTEST_MAIN(TimerControlTest)

#include "timercontroltest.moc"
