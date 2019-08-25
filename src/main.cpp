/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2019 Volker Kohaupt
 * 
 * Author:
 *      Volker Kohaupt <vkohaupt@freenet.de>
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * --End_License--
 */

#include "mainwindow.h"
#include "global.h"

#include <QTranslator>
#include <QLibraryInfo>

#include <QDir>
#include <QtGlobal>

#include <gst/gst.h>

int main(int argc, char *argv[])
{
/*
    QByteArray env = qgetenv( "LD_LIBRARY_PATH" );
    qDebug() << "env:" << env;
    QString libraryInfo = QLibraryInfo::location( QLibraryInfo::LibrariesPath );
    qDebug() << "libraryInfo:" << libraryInfo;

    QByteArray appDir = QDir::currentPath().toLatin1();
    bool a = qputenv( "LD_LIBRARY_PATH", appDir + "/lib" + ":" + libraryInfo.toLatin1() );

    env = qgetenv( "LD_LIBRARY_PATH" );
    qDebug() << env;
*/
    QApplication app(argc, argv);

    QStringList arguments = QApplication::instance()->arguments();
    if ( !arguments.empty() and ( arguments.count() == 2 ) )
    {
        QStringList arguments = QApplication::instance()->arguments();
        if ( ( arguments.at(1) == "--help" ) or
             ( arguments.at(1) == "-h"     ) )
            {
                qDebug(" ");
                qDebug().noquote() << "Usage:" << global::name << "[Option] [File]";
                qDebug(" ");
                qDebug().noquote() << "Options:";
                qDebug().noquote() << "  -h or --help        Print this message";
                qDebug().noquote() << "  -v or --version     Print" << global::name << "version";
                return 0;
            }

        if ( ( arguments.at(1) == "--version" ) or ( arguments.at(1) == "-v" ) )
        {
            qDebug().noquote() << global::name << global::version;
            return 0;
        }

        QFile file( arguments.at(1) );
        if ( file.exists() == false )
        {
            qDebug().noquote() << global::nameOutput << arguments.at(1) << "file not exists";
            return 1;
        }
    }

/*
 * Under Windows the icon theme breeze is used https://github.com/KDE/breeze-icons.
 * The subfolder icons will be renamed to breez and copied to the windows distribution folder.
 * That's all to display the icons.
*/
#ifdef Q_OS_WIN
    QIcon::setThemeName( "breeze" );
#endif

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    // Initialize GStreamer
    gst_init (&argc, &argv);

    QTranslator * qtTranslator = new QTranslator();
    qtTranslator->load( "qt_" + QLocale::system().name(), QLibraryInfo::location( QLibraryInfo::TranslationsPath ) );
    app.installTranslator( qtTranslator );

    QTranslator translator;
    translator.load( QLocale::system().name(), ":/language" );
    app.installTranslator( &translator );

    QvkMainWindow w;
    w.show();

    return app.exec();
}
