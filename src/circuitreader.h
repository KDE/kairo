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
