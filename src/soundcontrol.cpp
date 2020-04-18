/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

*/

#include "soundcontrol.h"

#include <QMediaPlayer>

SoundControl::SoundControl(QObject *parent)
    : SoundControlInterface{parent},
      m_player{new QMediaPlayer{this}}
{
}

void SoundControl::playShortBeep()
{
    m_player->setMedia(QUrl("qrc:/shortbeep.wav"));
    m_player->play();
}

void SoundControl::playLongBeep()
{
    m_player->setMedia(QUrl("qrc:/longbeep.wav"));
    m_player->play();
}

void SoundControl::playEndBeep()
{
    m_player->setMedia(QUrl("qrc:/endbeep.wav"));
    m_player->play();
}
