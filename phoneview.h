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

#pragma once

#include <QQuickWindow>

namespace KDeclarative
{
    class QmlObject;
} // namespace KDeclarative

class SimApi;


class DeviceView : public QQuickWindow
{
    Q_OBJECT

public:
    DeviceView(const QSize &size);

public Q_SLOTS:
    void loadQmlPackage(const QString &packagePath);
    void loadShellPackage(const QString &packagePath);

private:
    SimApi *m_simApi;
    KDeclarative::QmlObject *m_qmlObj;
};
