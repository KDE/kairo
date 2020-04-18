/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

*/

#ifndef SOUNDCONTROL_H
#define SOUNDCONTROL_H

#include "soundcontrolinterface.h"

class QMediaPlayer;

class SoundControl : public SoundControlInterface
{
    Q_OBJECT
public:
    explicit SoundControl(QObject *parent = nullptr);

public slots:
    void playShortBeep() override;
    void playLongBeep() override;
    void playEndBeep() override;

private:
    QMediaPlayer *m_player;
};

#endif
