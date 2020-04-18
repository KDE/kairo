/* This file is part of Kairo Timer

   SPDX-FileCopyrightText: 2016 (c) Kevin Ottens <ervin@kde.org>

   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

*/

#include "soundcontrolstub.h"

SoundControlStub::SoundControlStub(QObject *parent)
    : SoundControlInterface{parent}
{
}

QVector<SoundControlStub::SoundType> SoundControlStub::calls() const
{
    return m_calls;
}

void SoundControlStub::clear()
{
    m_calls.clear();
}

void SoundControlStub::playShortBeep()
{
    m_calls << ShortBeep;
}

void SoundControlStub::playLongBeep()
{
    m_calls << LongBeep;
}

void SoundControlStub::playEndBeep()
{
    m_calls << EndBeep;
}
