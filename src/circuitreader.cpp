/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

*/

#include "circuitreader.h"

#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTextStream>

#include <functional>

CircuitReader::CircuitReader(QIODevice *device)
    : m_device{device}
{
}

CircuitReader::CircuitReader(CircuitReader &&other)
    : m_device{other.m_device},
      m_metaData{other.m_metaData}
{
    other.m_device = nullptr;
    other.m_metaData.clear();
}

CircuitReader &CircuitReader::operator=(CircuitReader &&other)
{
    m_device = other.m_device;
    m_metaData = other.m_metaData;

    other.m_device = nullptr;
    other.m_metaData.clear();

    return *this;
}

QIODevice *CircuitReader::device() const
{
    return m_device;
}

CircuitReader::MetaData CircuitReader::readMetaData()
{
    if (!m_device)
        return {};

    if (!m_device->isOpen() && !m_device->open(QIODevice::ReadOnly))
        return {};

    if (m_device->pos() > 0)
        return m_metaData;

    auto line = QString::fromUtf8(m_device->readLine()).trimmed();
    if (line != QStringLiteral("---"))
        return {};

    auto metaData = MetaData{};
    line = QString::fromUtf8(m_device->readLine()).trimmed();
    while (!m_device->atEnd() && line != QStringLiteral("---")) {
        const auto columnIndex = line.indexOf(':');
        if (columnIndex < 0)
            return {};

        const auto key = line.left(columnIndex).simplified();
        const auto value = line.mid(columnIndex + 1).simplified();
        metaData.insert(key, value);

        line = QString::fromUtf8(m_device->readLine()).trimmed();
    }

    if (line != QStringLiteral("---"))
        return {};

    m_metaData = metaData;

    return m_metaData;
}

namespace {
    bool isLoopObject(const QJsonObject &object)
    {
        return object.contains("content") && object.contains("count");
    }

    bool isTimerObject(const QJsonObject &object)
    {
        return object.contains("text") && !isLoopObject(object);
    }

    TimerModel convertTimerObject(const QJsonObject &object)
    {
        return {
            object.value("text").toString(),
            object.value("duration").toInt()
        };
    }

    QVector<TimerModel> convertLoopObject(const QJsonObject &object)
    {
        const auto count = object.value("count").toInt();
        if (count <= 0)
            return {};

        const auto content = object.value("content");
        if (!content.isArray())
            return {};

        auto array = content.toArray();
        if (array.isEmpty())
            return {};

        const auto end = std::remove_if(array.begin(), array.end(),
                                        [] (const QJsonValue &value) {
                                            return !value.isObject() || !isTimerObject(value.toObject());
                                        });

        const auto text = object.value("text").toString();
        const auto prefix = text.isEmpty() ? QString{} : text + ", ";

        auto loop = QVector<TimerModel>{};

        for (int i = 0; i < count; i++) {
            auto timers = QVector<TimerModel>{};
            std::transform(array.begin(), end, std::back_inserter(timers),
                           [prefix, i, count] (const QJsonValue &value) {
                               const auto timer = convertTimerObject(value.toObject());
                               return TimerModel{
                                   QString("%1%2 (%3/%4)").arg(prefix).arg(timer.text()).arg(i + 1).arg(count),
                                   timer.duration()
                               };
                           });
            loop += timers;
        }

        const auto removeLast = object.value("removeLast").toBool();
        if (removeLast)
            loop.removeLast();

        return loop;
    }
}

CircuitModel CircuitReader::readCircuit()
{
    if (!m_device)
        return {};

    if (!m_device->isOpen())
        readMetaData();

    auto content = QVector<TimerModel>{};

    const auto data = m_device->readAll();
    const auto json = QJsonDocument::fromJson(data);
    if (json.isArray()) {
        const auto array = json.array();
        const auto end = std::remove_if(array.begin(), array.end(),
                                        [] (const QJsonValue &value) { return !value.isObject(); });

        auto objects = QVector<QJsonObject>{};
        auto toObject = std::mem_fn(static_cast<QJsonObject(QJsonValue::*)() const>(&QJsonValue::toObject));
        std::transform(array.begin(), end, std::back_inserter(objects), toObject);

        content = std::accumulate(objects.begin(), objects.end(), content,
                                  [] (const QVector<TimerModel> &content, const QJsonObject &object) {
                                      auto timers = content;
                                      if (isTimerObject(object))
                                          timers << convertTimerObject(object);
                                      else if (isLoopObject(object))
                                          timers += convertLoopObject(object);
                                      return timers;
                                  });
    }

    return {m_metaData.value("name").toString(), content};
}
