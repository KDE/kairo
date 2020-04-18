/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

*/

#ifndef CIRCUITMODEL_H
#define CIRCUITMODEL_H

#include <initializer_list>

#include <QVector>

#include "timermodel.h"

class CircuitModel
{
    Q_GADGET
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(bool empty READ isEmpty)
    Q_PROPERTY(int size READ size)
public:
    CircuitModel();
    CircuitModel(const QString &name, std::initializer_list<TimerModel> content);
    CircuitModel(const QString &name, const QVector<TimerModel> &content);

    bool operator==(const CircuitModel &other) const;
    bool operator!=(const CircuitModel &other) const;

    QString name() const;
    bool isEmpty() const;
    int size() const;

    Q_INVOKABLE TimerModel at(int index) const;

private:
    QString m_name;
    QVector<TimerModel> m_content;
};

Q_DECLARE_METATYPE(CircuitModel)

#endif
