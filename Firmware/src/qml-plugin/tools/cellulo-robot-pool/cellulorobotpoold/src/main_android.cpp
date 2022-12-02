/*
 * Copyright (C) 2017 EPFL
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#include <QCoreApplication>
#include <QAndroidService>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QLoggingCategory>
#include <string.h>

#include <CelluloLocalRelayServer.h>

using namespace Cellulo;

int main(int argc, char** argv){

    //GUI
    if(argc <= 1){
        QGuiApplication app(argc, argv);

        qInfo() << "cellulorobotpoold main(): GUI starting...";

        QQmlApplicationEngine engine;
        engine.load(QUrl(QStringLiteral("qrc:///src/main_android.qml")));

        return app.exec();
    }

    //Service
    else if(argc > 1 && strcmp(argv[1], "-service") == 0){
        QAndroidService app(argc, argv);

        qInfo() << "cellulorobotpoold main(): Service starting...";

        QLoggingCategory::setFilterRules(QStringLiteral("qt.bluetooth* = true"));

        CelluloLocalRelayServer server;
        server.setListening(true);

        return app.exec();
    }

    //Unrecognized argument
    else{
        qWarning() << "cellulorobotpoold main(): Unrecognized command line argument.";
        return -1;
    }
}
