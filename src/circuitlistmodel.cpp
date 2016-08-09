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

#include "circuitlistmodel.h"

#include <QDebug>
#include <QFileSystemWatcher>
#include <QStandardPaths>

#include <memory>

#include "circuitreader.h"

CircuitListModel::CircuitListModel(QObject *parent)
    : QAbstractListModel{parent},
      m_watcher{new QFileSystemWatcher{this}}
{
    auto dir = storageDir();
    if (!dir.exists())
        dir.mkpath(".");

    connect(m_watcher, &QFileSystemWatcher::directoryChanged,
            this, &CircuitListModel::onDirectoryContentChanged);
    connect(m_watcher, &QFileSystemWatcher::fileChanged,
            this, &CircuitListModel::onDirectoryContentChanged);
    m_watcher->addPath(dir.path());

    onDirectoryContentChanged();
}

QHash<int, QByteArray> CircuitListModel::roleNames() const
{
    auto roles = QAbstractListModel::roleNames();
    roles.insert(FilePathRole, "filePath");
    return roles;
}

int CircuitListModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_circuits.size();
    else
        return 0;
}

QVariant CircuitListModel::data(const QModelIndex &index, int role) const
{
    if (index.parent().isValid()
     || index.row() < 0
     || index.row() >= rowCount()
     || index.column() != 0) {
        return {};
    }

    auto circuitFile = m_circuits.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return circuitFile.name;
    case FilePathRole:
        return circuitFile.filePath;
    default:
        return {};
    }
}

CircuitModel CircuitListModel::loadCircuit(int index)
{
    if (index < 0 || index >= m_circuits.size()) {
        qWarning() << "Wrong index passed to loadCircuit";
        return {};
    }

    const auto path = m_circuits.at(index).filePath;
    auto file = std::make_unique<QFile>(path);
    auto reader = CircuitReader{file.get()};
    return reader.readCircuit();
}

QString CircuitListModel::path() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}

void CircuitListModel::onDirectoryContentChanged()
{
    const auto entries = storageDir().entryInfoList({"*.kairo"}, QDir::Files|QDir::Readable);
    auto circuits = QVector<CircuitFile>{};
    std::transform(entries.constBegin(), entries.constEnd(),
                   std::back_inserter(circuits),
                   [] (const QFileInfo &entry) {
                       const auto path = entry.absoluteFilePath();
                       auto file = std::make_unique<QFile>(path);
                       auto reader = CircuitReader{file.get()};
                       const auto metaData = reader.readMetaData();
                       return CircuitFile{path, metaData.value("name").toString()};
                   });
    std::sort(circuits.begin(), circuits.end());

    if (m_circuits != circuits) {
        beginResetModel();
        m_circuits = circuits;
        endResetModel();
    }
}

QDir CircuitListModel::storageDir()
{
    return {path()};
}

bool CircuitListModel::CircuitFile::operator<(const CircuitListModel::CircuitFile &other) const
{
    return name < other.name;
}

bool CircuitListModel::CircuitFile::operator==(const CircuitListModel::CircuitFile &other) const
{
    return name == other.name
        && filePath == other.filePath;
}
