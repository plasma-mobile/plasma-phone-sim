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

#include <Plasma/PluginLoader>
#include <Plasma/Svg>

#include "packages.h"
#include "simapi.h"

DeviceView::DeviceView(const QSize &size, const QString &deviceSvgPath)
    : QQuickWindow(),
      m_simApi(new SimApi(QString(), this)),
      m_qmlObj(0),
      m_parentItem(0),
      m_frameComponent(0),
      m_frameEngine(0),
      m_lnfPackageStructure(0),
      m_deviceSvg(0)
{
    // resize to the emulated resolution
    // this gives the (approximate) physical size of the device on the local screen
    resize(size);
    setMinimumSize(size);
    setMaximumSize(size);

    connect(m_simApi, &SimApi::packagePathChanged,
            this, &DeviceView::loadQmlPackage);

    createDeviceSvg(deviceSvgPath);
}

DeviceView::~DeviceView()
{
    delete m_lnfPackageStructure;
}

void DeviceView::createDeviceSvg(const QString &deviceSvgPath)
{
    if (deviceSvgPath.isEmpty()) {
        qDebug() << "No device SVG ... experience degraded.";
        return;
    }

    if (m_deviceSvg) {
        //TODO: make it possible to replace the svg at runtime for fast
        //      device switching
        qDebug() << "Device SVG already set!";
        return;
    }

    m_deviceSvg = new Plasma::Svg(this);
    m_deviceSvg->setImagePath(deviceSvgPath);
    m_deviceSvg->resize(size());
    m_screenGeom = m_deviceSvg->elementRect("device-screen");
    //qDebug() << m_screenGeom << size;

    QString frameQml = QString(
        "import QtQuick 2.0\n"
        "import org.kde.plasma.core 2.0 as PlasmaCore\n\n"
        "PlasmaCore.SvgItem { \n"
        "    id: frame\n"
        "    svg: PlasmaCore.Svg { imagePath: \"%1\" } \n"
        "    anchors.fill: parent\n"
        "    Rectangle {\n"
        "        id: screen\n"
        "        color: \"black\"\n"
        "        x: %2\n"
        "        y: %3\n"
        "        width: %4\n"
        "        height: %5\n"
        "        clip: true\n"
        "    }");

    // insert the values for the svg path and screen geometry
    frameQml = frameQml.arg(deviceSvgPath)
                       .arg(m_screenGeom.left())
                       .arg(m_screenGeom.top())
                       .arg(m_screenGeom.width())
                       .arg(m_screenGeom.height());

    QMetaEnum values = m_simApi->metaObject()->enumerator(m_simApi->metaObject()->indexOfEnumerator("Sensor"));
    SimApi::Sensors sensors;
    for (int i = 0; i < values.keyCount(); ++i) {
        if (m_deviceSvg->hasElement(values.key(i))) {
            sensors |= (SimApi::Sensor)values.value(i);
        }
    }
    m_simApi->setSensors(sensors);

    // hardware keys, including the generated QML for them
    values = m_simApi->metaObject()->enumerator(m_simApi->metaObject()->indexOfEnumerator("HardwareKey"));

    // techincally the Rectangle is not needed
    // but it can be nice for debuggig to see where the buttons are
    const QString hwKeyQml("\n    Rectangle { \n"
//                           "          color: \"green\"\n"
                           "          opacity: 0\n"
                           "          id: %1\n"
                           "          x: %2; y: %3; z: 1000\n"
                           "          width: %4; height: %5\n"
                           "          MouseArea {\n"
                           "             anchors.fill: parent\n"
                           "              onPressed: { api.%6Pressed(); }\n"
                           "              onReleased: { api.%7Released(); }\n"
                           "          }\n"
                           "    }");

    SimApi::HardwareKeys keys;
    for (int i = 0; i < values.keyCount(); ++i) {
        if (m_deviceSvg->hasElement(values.key(i))) {
            keys |= (SimApi::HardwareKey)values.value(i);

            // now insert the QML bits
            QRectF keyRect = m_deviceSvg->elementRect(values.key(i));
            QString lowerCaseName = values.key(i);
            lowerCaseName = lowerCaseName.left(1).toLower() +
                            lowerCaseName.right(lowerCaseName.size() - 1);
            frameQml += hwKeyQml.arg(lowerCaseName)
                                .arg(keyRect.x())
                                .arg(keyRect.y())
                                .arg(keyRect.width())
                                .arg(keyRect.height())
                                .arg(lowerCaseName)
                                .arg(lowerCaseName);
        }
    }
    m_simApi->setHardwareKeys(keys);

    // append the closing brace for the frame item
    frameQml += "\n}";

    //qDebug() << frameQml;
    m_frameEngine = new QQmlEngine(this);
    m_frameComponent = new QQmlComponent(m_frameEngine);
    m_frameComponent->setData(frameQml.toUtf8(), QUrl());
    m_frameEngine->rootContext()->setContextProperty("api", m_simApi);

    if (m_frameComponent->isError()) {
        qDebug() << "INTERNAL QML ERROR: " << m_frameComponent->errors();
    } else if (m_frameComponent->isReady()) {
        createFrame(QQmlComponent::Ready);
    } else {
        connect(m_frameComponent, &QQmlComponent::statusChanged,
                this, &DeviceView::createFrame);
    }

    setColor(Qt::transparent);
    setFlags(Qt::FramelessWindowHint);
    setClearBeforeRendering(true);
}

void DeviceView::createFrame(QQmlComponent::Status status)
{
    if (status == QQmlComponent::Ready) {
        m_parentItem = static_cast<QQuickItem *>(m_frameComponent->create(m_frameEngine->rootContext()));
        m_parentItem->setParentItem(contentItem());
        m_parentItem = m_parentItem->childItems().first();

        Q_ASSERT(m_parentItem);
        if (m_qmlObj) {
            QQuickItem *mainItem = static_cast<QQuickItem *>(m_qmlObj->rootObject());
            mainItem->setParentItem(m_parentItem);
        }
    }
}

void DeviceView::setLookAndFeelPackge(const QString &packagePath)
{
    if (!m_lnfPackageStructure) {
        m_lnfPackageStructure = new LookAndFeelPackage();
    }

    m_lnfPackage = Plasma::Package(m_lnfPackageStructure);
    m_lnfPackage.setPath(packagePath);
}

void DeviceView::loadLookAndFeelComponent(const QString &component)
{
    if (!m_lnfPackage.isValid()) {
        qDebug() << "No Look And Feel package has been set.";
        return;
    }

    const QString fileKey = component + "mainscript";
    const QString qml = m_lnfPackage.filePath(fileKey.toLatin1());
    if (qml.isEmpty()) {
        qDebug() << "Could not find starting file for component" << component
                 << "in Look And Feel package at" << m_lnfPackage.path();
        return;
    }

    loadQmlPackage(qml);
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

    if (sender() != m_simApi || packagePath != path) {
        disconnect(m_simApi, &SimApi::packagePathChanged, this, &DeviceView::loadQmlPackage);
        m_simApi->setPackagePath(packagePath);
        connect(m_simApi, &SimApi::packagePathChanged, this, &DeviceView::loadQmlPackage);
    }

    m_qmlObj->engine()->rootContext()->setContextProperty("api", m_simApi);

    qDebug() << "Loading QML from:" << path;
    m_qmlObj->setSource(QUrl::fromLocalFile(main));

    QVariantHash initialProperties;
    //initialProperties["x"] = m_screenGeom.x();
    //initialProperties["y"] = m_screenGeom.y();
    initialProperties["width"] = m_screenGeom.width();
    initialProperties["height"] = m_screenGeom.height();
    m_qmlObj->completeInitialization(initialProperties);

    QQuickItem *mainItem = static_cast<QQuickItem *>(m_qmlObj->rootObject());
    //qDebug() << "Setting to" << m_parentItem;
    mainItem->setParentItem(m_parentItem ? m_parentItem : contentItem());
}

void DeviceView::loadShellPackage(const QString &packagePath)
{
    m_shellPackage = Plasma::PluginLoader::self()->loadPackage("Plasma/Shell");
    m_shellPackage.setPath(packagePath);
    if (!m_shellPackage.isValid()) {
        qDebug() << "Invalid shell package!";
        return;
    }

    qDebug() << "Loading shell package at:" << packagePath;
}

#include "moc_deviceview.cpp"
