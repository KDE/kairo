/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

*/

#ifndef SOUNDCONTROLINTERFACE_H
#define SOUNDCONTROLINTERFACE_H

#include <QObject>

class SoundControlInterface : public QObject
{
    Q_OBJECT
public:
    explicit SoundControlInterface(QObject *parent = nullptr);

public slots:
    virtual void playShortBeep() = 0;
    virtual void playLongBeep() = 0;
    virtual void playEndBeep() = 0;
};

#endif
