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

#include "phoneview.h"

#include <KDeclarative/KDeclarative>

PhoneView::PhoneView(const QSize &size)
    : QQuickView()
{
    setResizeMode(SizeRootObjectToView);

    // resize to the emulated resolution
    // this gives the (approximate) physical size of the device on the local screen
    resize(size);
    setMinimumSize(size);
    setMaximumSize(size);

    KDeclarative::KDeclarative kdeclarative;
    kdeclarative.setDeclarativeEngine(engine());
    kdeclarative.setupBindings();
    //kdeclarative.setTranslationDomain();
}

void PhoneView::loadQmlPackage(const QString &packagePath)
{
    if (packagePath.isEmpty()) {
        return;
    }
}

void PhoneView::loadShellPackage(const QString &packagePath)
{
    if (packagePath.isEmpty()) {
        return;
    }

}

#include "moc_phoneview.cpp"
