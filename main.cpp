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

#include <QApplication>
#include <QCommandLineParser>
#include <QQuickWindow>
#include <QDebug>

#include <klocalizedstring.h>

static const char version[] = "0.1";

int main(int argc, char *argv[])
{
    KLocalizedString::setApplicationDomain("plasma-phone-sim");

    QCommandLineParser parser;
    QApplication app(argc, argv);
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

    return app.exec();
}
