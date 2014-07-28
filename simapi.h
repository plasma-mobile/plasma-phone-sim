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

#include <QObject>
#pragma once

class SimApi : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString packagePath READ packagePath WRITE setPackagePath NOTIFY packagePathChanged)

public:
    SimApi(const QString &currentPath, QObject *parent);

    QString packagePath() const;
    void setPackagePath(const QString &path);

Q_SIGNALS:
    void packagePathChanged(const QString &path);

private:
    QString m_packagePath;
};