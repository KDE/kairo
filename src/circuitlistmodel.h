/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

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
