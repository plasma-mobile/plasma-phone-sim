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

#include <QFileInfo>
#include <QQuickItem>

#include <KDeclarative/KDeclarative>
#include <KDeclarative/QmlObject>

#include <Plasma/Package>
#include <Plasma/PluginLoader>

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
    QString main;
    const QFileInfo info(packagePath);

    if (info.isFile()) {
        main = packagePath;
    } else if (info.isDir()) {
        Plasma::Package package = Plasma::PluginLoader::self()->loadPackage("Plasma/Generic");
        package.setPath(packagePath);
        main = package.filePath("mainscript");
    }

    if (main.isEmpty()) {
        qDebug() << "No QML specified for loading";
        return;
    }

    qDebug() << "Loading QML from:" << packagePath;
    setSource(main);
    /*
    KDeclarative::QmlObject *qmlObj = new KDeclarative::QmlObject(this);
    qmlObj->setInitializationDelayed(true);
    qmlObj->setSource(QUrl::fromLocalFile(main));
    qmlObj->completeInitialization();
    qqItem->setWidth(root->width());
    qqItem->setHeight(root->height());
    qqItem->setParentItem(rootObject());
    */
}

void PhoneView::loadShellPackage(const QString &packagePath)
{
    Plasma::Package package = Plasma::PluginLoader::self()->loadPackage("Plasma/Shell");
    package.setPath(packagePath);
    if (!package.isValid()) {
        qDebug() << "Invalid shell package!";
        return;
    }

    qDebug() << "Loading shell package at:" << packagePath;
}

#include "moc_phoneview.cpp"
