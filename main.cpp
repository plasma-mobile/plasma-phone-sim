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

#include <QCommandLineParser>
#include <QDebug>
#include <QApplication>
#include <QPaintDevice>
#include <QScreen>
#include <QQuickWindow>

#include <Plasma/PluginLoader>
#include <Plasma/Package>
#include <Plasma/Svg>

#include <klocalizedstring.h>

#include "deviceview.h"

static const char version[] = "0.1";

int main(int argc, char *argv[])
{
    KLocalizedString::setApplicationDomain("plasma-phone-sim");

    QCommandLineParser parser;
    QApplication app(argc, argv);
    app.setApplicationName("plasma-phone-sim");
    app.setApplicationDisplayName(i18n("Plasma Device Simulator"));
    app.setOrganizationDomain("kde.org");
    app.setApplicationVersion(version);
    app.setWindowIcon(QIcon::fromTheme("plasma")); //TODO: phone icon? :)
    parser.setApplicationDescription("Plasma Device Simulator");

    QCommandLineOption qmlPackageOpt(QStringList() << QStringLiteral("q") << QStringLiteral("qml-package"),
                                     i18n("Path to a QML package to load"),
                                     i18n("QML package path"));

    QCommandLineOption shellPackageOpt(QStringList() << QStringLiteral("s") << QStringLiteral("shell-package"),
                                       i18n("A Plasma Shell package to load"),
                                       i18n("shell package path"));

    QCommandLineOption resOpt(QStringList() << QStringLiteral("r") << QStringLiteral("dev-res"),
                              i18n("Resolution to emultate: WIDTHxHEIGHT@DPI"),
                              QStringLiteral("resolution"),
                              QStringLiteral("1080x1920@445"));

    QCommandLineOption deviceOpt(QStringList() << QStringLiteral("d") << QStringLiteral("device"),
                                 i18n("Name of device to emulate"),
                                 QStringLiteral("device"),
                                 QStringLiteral("nexus5"));

    QCommandLineOption listDevicesOpt(QStringLiteral("list-devices"), i18n("Name of supported devices"));

    parser.addVersionOption();
    parser.addHelpOption();
    parser.addOption(qmlPackageOpt);
    parser.addOption(shellPackageOpt);
    parser.addOption(resOpt);
    parser.addOption(deviceOpt);
    parser.addOption(listDevicesOpt);
    parser.process(app);

    if (parser.isSet(listDevicesOpt)) {
        Plasma::Package package = Plasma::PluginLoader::self()->loadPackage("Plasma/Generic");
        package.removeDefinition("mainscript");
        package.setPath("org.kde.plasmadevicesim.screens");
        const QStringList devices = package.entryList("images");
        QMap<QString, QString> sortedDevices;
        for (auto device: devices) {
            sortedDevices.insert(device.left(device.lastIndexOf('.')), device);
        }
        qDebug() << "Supported devices include:" << QStringList(sortedDevices.keys()).join(QStringLiteral(", "));
        exit(0);
    }

    QString svgPath;
    QSize frameSize;
    int width = 0;
    int height = 0;
    int nativeDpi = app.primaryScreen()->logicalDotsPerInchX();
    int deviceDpi = nativeDpi;
    if (parser.isSet(resOpt)) {
        const QString res = parser.value(resOpt);
        const int xIndex = res.indexOf('x');
        if (xIndex != -1) {
            width = qMax(300, res.left(xIndex).toInt());
            const int atIndex = res.indexOf('@');
            height = qMax(500, res.mid(xIndex + 1, atIndex - xIndex - 1).toInt());
            if (atIndex > 0) {
                deviceDpi = res.right(res.length() - atIndex - 1).toInt();
            }
        }
    } else {
        const QString device = parser.value(deviceOpt);

        Plasma::Package package = Plasma::PluginLoader::self()->loadPackage("Plasma/Generic");
        package.removeDefinition("mainscript");
        package.setPath("org.kde.plasmadevicesim.screens");
        svgPath = package.filePath("images", device + ".svgz");
        if (svgPath.isEmpty()) {
            svgPath = package.filePath("images", device + ".svg");

            if (svgPath.isEmpty()) {
                const QString errorMsg = i18n("Could not find requested device %1. Try --list-devices.", device);
                qFatal(errorMsg.toLocal8Bit());
            }
        }

        Plasma::Svg svg;
        svg.setImagePath(svgPath);
        frameSize = svg.size();
        width = svg.elementSize("device-screen").width();
        height = svg.elementSize("device-screen").height();
        if (svg.hasElement("screen-res")) {
            deviceDpi = svg.elementSize("screen-res").width();
        }
    }

    qDebug() << "Going to emulate resolution" << width << 'x' << height << '@' << deviceDpi
             << "at native dpi of" << nativeDpi;
    float ratio = 1;
    if (nativeDpi != deviceDpi) {
        ratio = nativeDpi / float(deviceDpi);
    }

    QSize size;
    if (frameSize.isEmpty()) {
        size = QSize(width * ratio, height * ratio);
    } else {
        size = QSize(frameSize.width() * ratio, frameSize.height() * ratio);
    }

    qDebug() << "Emulated resolution" << size << '@' << deviceDpi;
    DeviceView window(size, svgPath);
    window.show();

    const QString shellPackage = parser.value(shellPackageOpt);
    if (shellPackage.isEmpty()) {
        // no shell package, lets see if we have a QML package then
        const QString qmlPackage = parser.value(qmlPackageOpt);
        window.loadQmlPackage(qmlPackage);
    } else {
        window.loadShellPackage(shellPackage);
    }


    return app.exec();
}
