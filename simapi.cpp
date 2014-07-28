/*
 *  Copyright 2014 Aaron Seigo <aseigo@kde.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "simapi.h"

SimApi::SimApi(const QString &currentPath, QObject *parent)
    : QObject(parent),
      m_packagePath(currentPath)
{
}

QString SimApi::packagePath() const
{
    return m_packagePath;
}

void SimApi::setPackagePath(const QString &path)
{
    if (path != m_packagePath) {
        m_packagePath = path;
        emit packagePathChanged(m_packagePath);
    }
}


#include "simapi.moc"
