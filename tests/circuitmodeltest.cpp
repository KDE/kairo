/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

*/

#include <QtTest>

#include "circuitmodel.h"

static_assert(std::is_copy_assignable<CircuitModel>::value, "Should be copy assignable");
static_assert(std::is_copy_constructible<CircuitModel>::value, "Should be copy constructible");
static_assert(std::is_move_assignable<CircuitModel>::value, "Should be move assignable");
static_assert(std::is_move_constructible<CircuitModel>::value, "Should be move constructible");

class CircuitModelTest : public QObject
{
    Q_OBJECT
private slots:
    void shouldHaveDefaultState()
    {
        // GIVEN
        auto model = CircuitModel{};

        // THEN
        QCOMPARE(model.name(), QString());
        QCOMPARE(model.isEmpty(), true);
        QCOMPARE(model.size(), 0);
    }

    void shouldBeConstructibleFromVector()
    {
        // GIVEN
        const auto models = QVector<TimerModel>{
            {"run"},
            {"rest", 10},
            {"rest", 20}
        };
        const auto model = CircuitModel{"foo", models};

        // THEN
        QCOMPARE(model.name(), QString("foo"));
        QCOMPARE(model.isEmpty(), false);
        QCOMPARE(model.size(), 3);
        QCOMPARE(model.at(0), models.at(0));
        QCOMPARE(model.at(1), models.at(1));
        QCOMPARE(model.at(2), models.at(2));
    }

    void shouldBeConstructibleFromInitializerList()
    {
        // GIVEN
        const auto models = QVector<TimerModel>{
            {"run"},
            {"rest", 10},
            {"rest", 20}
        };
        const auto model = CircuitModel{
            "foo",
            {
                {"run"},
                {"rest", 10},
                {"rest", 20}
            }
        };

        // THEN
        QCOMPARE(model.name(), QString("foo"));
        QCOMPARE(model.isEmpty(), false);
        QCOMPARE(model.size(), 3);
        QCOMPARE(model.at(0), models.at(0));
        QCOMPARE(model.at(1), models.at(1));
        QCOMPARE(model.at(2), models.at(2));
    }

    void shouldBeCopyable()
    {
        // GIVEN
        auto model = CircuitModel{
            "baz",
            {
                {"rest", 10000}
            }
        };

        // WHEN
        model = CircuitModel{
            "bleh",
            {
                {"foo", 5000},
                {"bar"}
            }
        };

        // THEN
        QCOMPARE(model.name(), QString("bleh"));
        QCOMPARE(model.isEmpty(), false);
        QCOMPARE(model.size(), 2);
        QCOMPARE(model.at(0).text(), QString("foo"));
        QCOMPARE(model.at(0).duration(), 5000);
        QCOMPARE(model.at(1).text(), QString("bar"));
        QCOMPARE(model.at(1).duration(), 0);
    }

    void shouldBeComparable()
    {
        // GIVEN
        auto model1 = CircuitModel{
            "foo",
            {
                {"rest", 10000}
            }
        };
        auto model2 = CircuitModel{
            "foo",
            {
                {"rest", 10000}
            }
        };
        auto model3 = CircuitModel{
            "foo",
            {
                {"foo", 5000},
                {"bar"}
            }
        };
        auto model4 = CircuitModel{
            "bar",
            {
                {"foo", 5000},
                {"bar"}
            }
        };

        // THEN
        QCOMPARE((model1 == model2), true);
        QCOMPARE((model1 != model2), false);
        QCOMPARE((model1 == model3), false);
        QCOMPARE((model1 != model3), true);
        QCOMPARE((model1 == model3), false);
        QCOMPARE((model1 != model3), true);
        QCOMPARE((model3 == model4), false);
        QCOMPARE((model3 != model4), true);
    }
};

QTEST_APPLESS_MAIN(CircuitModelTest)

#include "circuitmodeltest.moc"
