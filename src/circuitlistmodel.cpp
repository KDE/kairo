/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

*/

#include "circuitlistmodel.h"

#include <QDebug>
#include <QFileSystemWatcher>
#include <QStandardPaths>

#include <memory>

#include "circuitreader.h"

#ifdef Q_OS_ANDROID
#include <QStorageInfo>
#endif

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
    auto file = std::unique_ptr<QFile>(new QFile(path));
    auto reader = CircuitReader{file.get()};
    return reader.readCircuit();
}

QString CircuitListModel::path() const
{
#ifndef Q_OS_ANDROID
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
#else
    auto volumes = QStorageInfo::mountedVolumes();
    auto paths = QStringList();
    paths.reserve(volumes.size());
    std::transform(volumes.constBegin(), volumes.constEnd(),
                   std::back_inserter(paths),
                   [] (const QStorageInfo &volume) {
                       return volume.rootPath();
                   });
    auto validPaths = QStringList();
    std::copy_if(paths.constBegin(), paths.constEnd(),
                 std::back_inserter(validPaths),
                 [] (const QString &path) {
                     return path.startsWith("/storage/") && !path.startsWith("/storage/emulated");
                 });
    return validPaths.first() + "/Android/Kairo";
#endif
}

void CircuitListModel::onDirectoryContentChanged()
{
    const auto entries = storageDir().entryInfoList({"*.kairo"}, QDir::Files|QDir::Readable);
    auto circuits = QVector<CircuitFile>{};
    std::transform(entries.constBegin(), entries.constEnd(),
                   std::back_inserter(circuits),
                   [] (const QFileInfo &entry) {
                       const auto path = entry.absoluteFilePath();
                       auto file = std::unique_ptr<QFile>(new QFile(path));
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
