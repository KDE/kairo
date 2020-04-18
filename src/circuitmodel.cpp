/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

*/

#include "circuitmodel.h"

CircuitModel::CircuitModel() = default;

CircuitModel::CircuitModel(const QString &name, std::initializer_list<TimerModel> content)
    : CircuitModel(name, QVector<TimerModel>(content))
{
}

CircuitModel::CircuitModel(const QString &name, const QVector<TimerModel> &content)
    : m_name(name),
      m_content(content)
{
}

bool CircuitModel::operator==(const CircuitModel &other) const
{
    return m_name == other.m_name
        && m_content == other.m_content;
}

bool CircuitModel::operator!=(const CircuitModel &other) const
{
    return !(*this == other);
}

QString CircuitModel::name() const
{
    return m_name;
}

bool CircuitModel::isEmpty() const
{
    return m_content.isEmpty();
}

int CircuitModel::size() const
{
    return m_content.size();
}

TimerModel CircuitModel::at(int index) const
{
    return m_content.at(index);
}
