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
#include <QGuiApplication>
#include <QPaintDevice>
#include <QScreen>
#include <QQuickWindow>

#include <klocalizedstring.h>

static const char version[] = "0.1";

int main(int argc, char *argv[])
{
    KLocalizedString::setApplicationDomain("plasma-phone-sim");

    QCommandLineParser parser;
    QGuiApplication app(argc, argv);
    app.setApplicationName("plasma-phone-sim");
    app.setApplicationDisplayName(i18n("Plasma Phone Device Simulator"));
    app.setOrganizationDomain("kde.org");
    app.setApplicationVersion(version);
    app.setWindowIcon(QIcon::fromTheme("plasma")); //TODO: phone icon? :)
    parser.setApplicationDescription("Plasma Phone Device Simulator");

    QCommandLineOption qmlPackageOpt(QStringList() << QStringLiteral("q") << QStringLiteral("qml-package"),
                                     i18n("Path to a QML package to load"));

    QCommandLineOption shellPackageOpt(QStringList() << QStringLiteral("s") << QStringLiteral("shell-package"),
                                       i18n("A Plasma Shell package to load"));

    QCommandLineOption resOpt(QStringList() << QStringLiteral("r") << QStringLiteral("dev-res"),
                              i18n("Resolution to emultate: WIDTHxHEIGHT@DPI"),
                              QStringLiteral("resolution"),
                              QStringLiteral("1080x1920@445"));
    QCommandLineOption shellPluginOpt(QStringList() << QStringLiteral("p") << QStringLiteral("shell-plugin"),
                                      i18n("Force loading the given shell plugin"),
                                      QStringLiteral("plugin"));

    parser.addVersionOption();
    parser.addHelpOption();
    parser.addOption(qmlPackageOpt);
    parser.addOption(shellPackageOpt);
    parser.addOption(resOpt);
    parser.process(app);

    int width = 0;;
    int height = 0;
    int nativeDpi = app.primaryScreen()->logicalDotsPerInchX();
    int deviceDpi = nativeDpi;
    const QString res = parser.value(resOpt);
    const int xIndex = res.indexOf('x');
    if (xIndex != -1) {
        width = res.left(xIndex).toInt();
        const int atIndex = res.indexOf('@');
        height = res.mid(xIndex + 1, atIndex - xIndex - 1).toInt();
        if (atIndex > 0) {
            deviceDpi = res.right(res.length() - atIndex - 1).toInt();
        }
    }

    width = qMax(width, 300);
    height = qMax(height, 500);
    qDebug() << "Going to emulate resolution" << width << 'x' << height << '@' << deviceDpi
             << "at native dpi of" << nativeDpi;
    if (nativeDpi != deviceDpi) {
        float ratio = nativeDpi / float(deviceDpi);
        width *= ratio;
        height *= ratio;
    }
    qDebug() << "Emulated resolution" << width << 'x' << height << '@' << deviceDpi;

    // make our window
    QQuickWindow window;

    // resize the window to the emulated resolution
    // this gives the (approximate) physical size of the device on the local screen
    const QSize size(width, height);
    window.resize(size);
    window.setMinimumSize(size);
    window.setMaximumSize(size);
    window.show();

    return app.exec();
}
