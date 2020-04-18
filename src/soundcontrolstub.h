/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

*/

#ifndef SOUNDCONTROLSTUB_H
#define SOUNDCONTROLSTUB_H

#include "soundcontrolinterface.h"

#include <QVector>

class SoundControlStub : public SoundControlInterface
{
    Q_OBJECT
public:
    enum SoundType {
        ShortBeep,
        LongBeep,
        EndBeep
    };

    explicit SoundControlStub(QObject *parent = nullptr);

    QVector<SoundType> calls() const;
    void clear();

public slots:
    void playShortBeep() override;
    void playLongBeep() override;
    void playEndBeep() override;

private:
    QVector<SoundType> m_calls;
};

#endif
