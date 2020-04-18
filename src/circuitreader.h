/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

*/

#ifndef CIRCUITREADER_H
#define CIRCUITREADER_H

#include <QHash>
#include <QVariant>

#include "circuitmodel.h"

class QIODevice;

class CircuitReader
{
public:
    using MetaData = QHash<QString, QVariant>;

    explicit CircuitReader(QIODevice *device = nullptr);
    CircuitReader(CircuitReader &&other);
    CircuitReader &operator=(CircuitReader &&other);

    QIODevice *device() const;

    MetaData readMetaData();
    CircuitModel readCircuit();

private:
    CircuitReader(const CircuitReader &) = delete;
    CircuitReader &operator=(const CircuitReader &) = delete;

    QIODevice *m_device;
    MetaData m_metaData;
};

#endif
