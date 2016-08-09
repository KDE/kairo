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

#include "circuitcontrol.h"
#include "timercontrol.h"

class CircuitControlTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    {
        qRegisterMetaType<TimerControl*>();
    }

    void shouldHaveDefaultState()
    {
        // GIVEN
        auto control = std::make_unique<CircuitControl>();

        // THEN
        QCOMPARE(control->model(), CircuitModel{});
        QCOMPARE(control->timerControl(), static_cast<TimerControl*>(nullptr));
        QCOMPARE(control->name(), QString());
        QCOMPARE(control->size(), 0);
        QCOMPARE(control->remaining(), 0);
        QCOMPARE(control->currentTimer(), TimerModel{});
    }

    void shouldKnowTimerControl()
    {
        // GIVEN
        auto control = std::make_unique<CircuitControl>();
        auto timerControlSpy = std::make_unique<QSignalSpy>(control.get(), &CircuitControl::timerControlChanged);
        auto timerControl = new TimerControl(control.get());

        // WHEN
        control->setTimerControl(timerControl);

        // THEN
        QCOMPARE(control->timerControl(), timerControl);
        QCOMPARE(timerControlSpy->size(), 1);
        QCOMPARE(timerControlSpy->takeFirst().first().value<TimerControl*>(), timerControl);

        // WHEN
        control->setTimerControl(timerControl);

        // THEN
        QCOMPARE(control->timerControl(), timerControl);
        QCOMPARE(timerControlSpy->size(), 0);
    }

    void shouldDeriveValuesFromModel_data()
    {
        QTest::addColumn<CircuitModel>("model");

        QTest::newRow("empty and unnamed") << CircuitModel{};
        QTest::newRow("empty and named") << CircuitModel{"foo", {}};
        QTest::newRow("with data") << CircuitModel{"foo", {{"rest", 10000}, {"run"}, {"rest", 5000}}};
    }

    void shouldDeriveValuesFromModel()
    {
        // GIVEN
        auto control = std::make_unique<CircuitControl>();
        auto modelSpy = std::make_unique<QSignalSpy>(control.get(), &CircuitControl::modelChanged);
        auto nameSpy = std::make_unique<QSignalSpy>(control.get(), &CircuitControl::nameChanged);
        auto sizeSpy = std::make_unique<QSignalSpy>(control.get(), &CircuitControl::sizeChanged);
        auto remainingSpy = std::make_unique<QSignalSpy>(control.get(), &CircuitControl::remainingChanged);
        auto currentTimerSpy = std::make_unique<QSignalSpy>(control.get(), &CircuitControl::currentTimerChanged);

        QFETCH(CircuitModel, model);

        // WHEN
        control->setModel(model);

        // THEN
        QCOMPARE(control->model(), model);
        QCOMPARE(control->name(), model.name());
        QCOMPARE(control->size(), model.size());
        QCOMPARE(control->remaining(), model.size());

        if (!model.name().isEmpty() || !model.isEmpty()) {
            QCOMPARE(modelSpy->size(), 1);
            QCOMPARE(modelSpy->takeFirst().first().value<CircuitModel>(), model);
        } else {
            QCOMPARE(modelSpy->size(), 0);
        }

        if (!model.name().isEmpty()) {
            QCOMPARE(nameSpy->size(), 1);
            QCOMPARE(nameSpy->takeFirst().first().toString(), model.name());
        } else {
            QCOMPARE(nameSpy->size(), 0);
        }

        if (!model.isEmpty()) {
            QCOMPARE(sizeSpy->size(), 1);
            QCOMPARE(sizeSpy->takeFirst().first().toInt(), model.size());

            QCOMPARE(remainingSpy->size(), 1);
            QCOMPARE(remainingSpy->takeFirst().first().toInt(), model.size());

            if (!model.name().isEmpty()) {
                QCOMPARE(currentTimerSpy->size(), 1);
                QCOMPARE(currentTimerSpy->takeFirst().first().value<TimerModel>(), model.at(0));
            } else {
                QCOMPARE(currentTimerSpy->size(), 0);
            }
        } else {
            QCOMPARE(sizeSpy->size(), 0);
            QCOMPARE(remainingSpy->size(), 0);

            if (!model.name().isEmpty()) {
                QCOMPARE(currentTimerSpy->size(), 1);
                QCOMPARE(currentTimerSpy->takeFirst().first().value<TimerModel>(), TimerModel{});
            } else {
                QCOMPARE(currentTimerSpy->size(), 0);
            }
        }

        // WHEN
        control->setModel(model);

        // THEN
        QCOMPARE(modelSpy->size(), 0);
        QCOMPARE(nameSpy->size(), 0);
        QCOMPARE(sizeSpy->size(), 0);
        QCOMPARE(remainingSpy->size(), 0);
        QCOMPARE(currentTimerSpy->size(), 0);
    }

    void shouldDeriveValuesFromMovedModel_data()
    {
        shouldDeriveValuesFromModel_data();
    }

    void shouldDeriveValuesFromMovedModel()
    {
        // GIVEN
        auto control = std::make_unique<CircuitControl>();
        auto modelSpy = std::make_unique<QSignalSpy>(control.get(), &CircuitControl::modelChanged);
        auto nameSpy = std::make_unique<QSignalSpy>(control.get(), &CircuitControl::nameChanged);
        auto sizeSpy = std::make_unique<QSignalSpy>(control.get(), &CircuitControl::sizeChanged);
        auto remainingSpy = std::make_unique<QSignalSpy>(control.get(), &CircuitControl::remainingChanged);
        auto currentTimerSpy = std::make_unique<QSignalSpy>(control.get(), &CircuitControl::currentTimerChanged);

        QFETCH(CircuitModel, model);

        // WHEN
        auto copy = model;
        control->setModel(std::move(copy)); // This should move it in place

        // THEN
        QCOMPARE(control->model(), model);
        QCOMPARE(control->name(), model.name());
        QCOMPARE(control->size(), model.size());
        QCOMPARE(control->remaining(), model.size());

        if (!model.name().isEmpty() || !model.isEmpty()) {
            QCOMPARE(modelSpy->size(), 1);
            QCOMPARE(modelSpy->takeFirst().first().value<CircuitModel>(), model);
        } else {
            QCOMPARE(modelSpy->size(), 0);
        }

        if (!model.name().isEmpty()) {
            QCOMPARE(nameSpy->size(), 1);
            QCOMPARE(nameSpy->takeFirst().first().toString(), model.name());
        } else {
            QCOMPARE(nameSpy->size(), 0);
        }

        if (!model.isEmpty()) {
            QCOMPARE(sizeSpy->size(), 1);
            QCOMPARE(sizeSpy->takeFirst().first().toInt(), model.size());

            QCOMPARE(remainingSpy->size(), 1);
            QCOMPARE(remainingSpy->takeFirst().first().toInt(), model.size());

            if (!model.name().isEmpty()) {
                QCOMPARE(currentTimerSpy->size(), 1);
                QCOMPARE(currentTimerSpy->takeFirst().first().value<TimerModel>(), model.at(0));
            } else {
                QCOMPARE(currentTimerSpy->size(), 0);
            }
        } else {
            QCOMPARE(sizeSpy->size(), 0);
            QCOMPARE(remainingSpy->size(), 0);

            if (!model.name().isEmpty()) {
                QCOMPARE(currentTimerSpy->size(), 1);
                QCOMPARE(currentTimerSpy->takeFirst().first().value<TimerModel>(), TimerModel{});
            } else {
                QCOMPARE(currentTimerSpy->size(), 0);
            }
        }


        // WHEN
        control->setModel(std::move(model));

        // THEN
        QCOMPARE(modelSpy->size(), 0);
        QCOMPARE(nameSpy->size(), 0);
        QCOMPARE(sizeSpy->size(), 0);
        QCOMPARE(remainingSpy->size(), 0);
        QCOMPARE(currentTimerSpy->size(), 0);
    }

    void shouldPushNextTimerOnFinishOrSkip()
    {
        // GIVEN
        auto control = std::make_unique<CircuitControl>();
        auto timerControl = new TimerControl(control.get());

        control->setModel({
                              "foo",
                              {
                                  {"mooh"},
                                  {"rest", 10000},
                                  {"run"},
                                  {"rest", 5000}
                              }
                          });
        auto remainingSpy = std::make_unique<QSignalSpy>(control.get(), &CircuitControl::remainingChanged);
        auto currentTimerSpy = std::make_unique<QSignalSpy>(control.get(), &CircuitControl::currentTimerChanged);
        auto finishedSpy = std::make_unique<QSignalSpy>(control.get(), &CircuitControl::circuitFinished);

        // WHEN
        control->nextTimer();

        // THEN
        QCOMPARE(control->currentTimer(), control->model().at(1));
        QCOMPARE(control->remaining(), 3);
        QCOMPARE(remainingSpy->size(), 1);
        QCOMPARE(remainingSpy->takeFirst().first().toInt(), 3);
        QCOMPARE(currentTimerSpy->size(), 1);
        QCOMPARE(currentTimerSpy->takeFirst().first().value<TimerModel>(), control->model().at(1));
        QCOMPARE(finishedSpy->size(), 0);

        // WHEN
        control->setTimerControl(timerControl);

        // THEN
        QCOMPARE(timerControl->model(), control->currentTimer());
        QCOMPARE(timerControl->model(), control->model().at(1));
        QCOMPARE(control->remaining(), 3);
        QCOMPARE(remainingSpy->size(), 0);
        QCOMPARE(currentTimerSpy->size(), 0);
        QCOMPARE(finishedSpy->size(), 0);

        // WHEN
        emit timerControl->timerFinished();

        // THEN
        QCOMPARE(timerControl->model(), control->currentTimer());
        QCOMPARE(timerControl->model(), control->model().at(2));
        QCOMPARE(control->remaining(), 2);
        QCOMPARE(remainingSpy->size(), 1);
        QCOMPARE(remainingSpy->takeFirst().first().toInt(), 2);
        QCOMPARE(currentTimerSpy->size(), 1);
        QCOMPARE(currentTimerSpy->takeFirst().first().value<TimerModel>(), control->model().at(2));
        QCOMPARE(finishedSpy->size(), 0);

        // WHEN
        control->nextTimer();

        // THEN
        QCOMPARE(timerControl->model(), control->currentTimer());
        QCOMPARE(timerControl->model(), control->model().at(3));
        QCOMPARE(control->remaining(), 1);
        QCOMPARE(remainingSpy->size(), 1);
        QCOMPARE(remainingSpy->takeFirst().first().toInt(), 1);
        QCOMPARE(currentTimerSpy->size(), 1);
        QCOMPARE(currentTimerSpy->takeFirst().first().value<TimerModel>(), control->model().at(3));
        QCOMPARE(finishedSpy->size(), 0);

        // WHEN
        control->nextTimer();

        // THEN
        QCOMPARE(timerControl->model(), control->currentTimer());
        QCOMPARE(timerControl->model(), TimerModel{});
        QCOMPARE(control->remaining(), 0);
        QCOMPARE(remainingSpy->size(), 1);
        QCOMPARE(remainingSpy->takeFirst().first().toInt(), 0);
        QCOMPARE(currentTimerSpy->size(), 1);
        QCOMPARE(currentTimerSpy->takeFirst().first().value<TimerModel>(), TimerModel{});
        QCOMPARE(finishedSpy->size(), 1);
    }
};

QTEST_MAIN(CircuitControlTest)

#include "circuitcontroltest.moc"
