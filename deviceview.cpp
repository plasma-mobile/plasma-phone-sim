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
#include <Plasma/Svg>

#include "simapi.h"

DeviceView::DeviceView(const QSize &size, const QString &frameSvgPath)
    : QQuickWindow(),
      m_simApi(0),
      m_qmlObj(0),
      m_parentItem(0),
      m_frameComponent(0),
      m_frameEngine(0)
{
    // resize to the emulated resolution
    // this gives the (approximate) physical size of the device on the local screen
    resize(size);
    setMinimumSize(size);
    setMaximumSize(size);

    if (!frameSvgPath.isEmpty()) {
        Plasma::Svg svg;
        svg.setImagePath(frameSvgPath);
        svg.resize(size);
        const QRectF screenGeom = svg.elementRect("device-screen");
        const int leftMargin = screenGeom.left();
        const int topMargin = screenGeom.top();
        const int rightMargin = size.width() - screenGeom.right();
        const int bottomMargin = size.height() - screenGeom.bottom();

        const QString frameQml = QString(
            "import QtQuick 2.0\n\
             import org.kde.plasma.core 2.0 as PlasmaCore\n\
             \n\
             PlasmaCore.SvgItem { \n\
                svg: PlasmaCore.Svg { imagePath: \"%1\" } \n\
                anchors.fill: parent\n\
                anchors.leftMargin: %2\n\
                anchors.topMargin: %3\n\
                anchors.rightMargin: %4\n\
                anchors.bottomMargin: %5 }")
        .arg(frameSvgPath).arg(leftMargin).arg(topMargin).arg(rightMargin).arg(bottomMargin);
        m_frameEngine = new QQmlEngine(this);
        m_frameComponent = new QQmlComponent(m_frameEngine);
        m_frameComponent->setData(frameQml.toUtf8(), QUrl());
        if (m_frameComponent->isError()) {
            qDebug() << "INTERNAL QML ERROR: " << m_frameComponent->errors();
        } else if (m_frameComponent->isReady()) {
            createFrame(QQmlComponent::Ready);
        } else {
            connect(m_frameComponent, &QQmlComponent::statusChanged,
                    this, &DeviceView::createFrame);
        }

        setClearBeforeRendering(true);
        setColor(Qt::transparent);
        setFlags(Qt::FramelessWindowHint);
    }
}

void DeviceView::createFrame(QQmlComponent::Status status)
{
    if (status == QQmlComponent::Ready) {
        m_parentItem = static_cast<QQuickItem *>(m_frameComponent->create(m_frameEngine->rootContext()));
        m_parentItem->setParentItem(contentItem());
    }
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
    mainItem->setParentItem(m_parentItem ? m_parentItem : contentItem());
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
