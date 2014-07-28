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

#include "deviceview.h"

#include <QDir>
#include <QFileInfo>
#include <QQmlContext>
#include <QQuickItem>

#include <KDeclarative/KDeclarative>
#include <KDeclarative/QmlObject>

#include <Plasma/Package>
#include <Plasma/PluginLoader>

#include "simapi.h"

DeviceView::DeviceView(const QSize &size)
    : QQuickWindow(),
      m_simApi(0),
      m_qmlObj(0)
{
    // resize to the emulated resolution
    // this gives the (approximate) physical size of the device on the local screen
    resize(size);
    setMinimumSize(size);
    setMaximumSize(size);

//    setClearBeforeRendering(true);
//    setColor(Qt::transparent);
//    setFlags(Qt::FramelessWindowHint);
}

void DeviceView::loadQmlPackage(const QString &packagePath)
{
    QString main;
    QString path;
    const QFileInfo info(packagePath);

    if (info.isFile()) {
        path = main = packagePath;
    } else if (info.isDir()) {
        Plasma::Package package = Plasma::PluginLoader::self()->loadPackage("Plasma/Generic");
        package.setPath(packagePath);
        path = package.path();
        main = package.filePath("mainscript");
    }

    if (main.isEmpty()) {
        Plasma::Package package = Plasma::PluginLoader::self()->loadPackage("Plasma/Generic");
        package.setPath("org.kde.plasmadevicesim.default");
        path = package.path();
        main = package.filePath("mainscript");
    }

    m_qmlObj->deleteLater();
    m_qmlObj = new KDeclarative::QmlObject(this);
    m_qmlObj->setInitializationDelayed(true);

    KDeclarative::KDeclarative kdeclarative;
    kdeclarative.setDeclarativeEngine(m_qmlObj->engine());
    kdeclarative.setupBindings();
    //kdeclarative.setTranslationDomain();

    if (!m_simApi) {
        m_simApi = new SimApi(path, this);
        connect(m_simApi, &SimApi::packagePathChanged, this, &DeviceView::loadQmlPackage);
    } else if (sender() != m_simApi || packagePath != path) {
        disconnect(m_simApi, &SimApi::packagePathChanged, this, &DeviceView::loadQmlPackage);
        m_simApi->setPackagePath(packagePath);
        connect(m_simApi, &SimApi::packagePathChanged, this, &DeviceView::loadQmlPackage);
    }

    m_qmlObj->engine()->rootContext()->setContextProperty("api", m_simApi);

    qDebug() << "Loading QML from:" << path;
    m_qmlObj->setSource(QUrl::fromLocalFile(main));

    QVariantHash initialProperties;
    initialProperties["width"] = width();
    initialProperties["height"] = height();
    m_qmlObj->completeInitialization(initialProperties);

    QQuickItem *mainItem = static_cast<QQuickItem *>(m_qmlObj->rootObject());
    mainItem->setParentItem(contentItem());
}

void DeviceView::loadShellPackage(const QString &packagePath)
{
    Plasma::Package package = Plasma::PluginLoader::self()->loadPackage("Plasma/Shell");
    package.setPath(packagePath);
    if (!package.isValid()) {
        qDebug() << "Invalid shell package!";
        return;
    }

    qDebug() << "Loading shell package at:" << packagePath;
}

#include "moc_deviceview.cpp"
