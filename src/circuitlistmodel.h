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

#ifndef CIRCUITLISTMODEL_H
#define CIRCUITLISTMODEL_H

#include <QAbstractListModel>
#include <QDir>

#include "circuitmodel.h"

class QFileSystemWatcher;

class CircuitListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path CONSTANT)
public:
    enum Role {
        FilePathRole = Qt::UserRole + 1
    };
    Q_ENUM(Role)

    explicit CircuitListModel(QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE CircuitModel loadCircuit(int index);

    QString path() const;

private slots:
    void onDirectoryContentChanged();

private:
    QDir storageDir();

    struct CircuitFile
    {
        QString filePath;
        QString name;

        bool operator<(const CircuitFile &other) const;
        bool operator==(const CircuitFile &other) const;
    };

    QFileSystemWatcher *m_watcher;
    QVector<CircuitFile> m_circuits;
};

#endif
