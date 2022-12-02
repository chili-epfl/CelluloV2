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

/**
 * @file main_linux.cpp
 * @brief Cellulo Relay Server daemon for Linux
 * @author Ayberk Özgür
 * @date 2017-03-20
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <cerrno>
#include <unistd.h>
#include <syslog.h>
#include <csignal>

#include <QtGlobal>
#include <QString>
#include <QByteArray>
#include <QDebug>
#include <QLoggingCategory>
#include <QCoreApplication>

#include <CelluloLocalRelayServer.h>

using namespace Cellulo;

void syslogOutput(QtMsgType type, QMessageLogContext const& context, QString const& msg){
    Q_UNUSED(context);

    QByteArray localMsg = msg.toLocal8Bit();

    int priority = LOG_DEBUG;
    switch(type){
    case QtDebugMsg:
        priority = LOG_DEBUG;
        break;

    case QtInfoMsg:
        priority = LOG_INFO;
        break;

    case QtWarningMsg:
        priority = LOG_WARNING;
        break;

    case QtCriticalMsg:
        priority = LOG_CRIT;
        break;

    case QtFatalMsg:
        priority = LOG_EMERG;
        break;
    }

    syslog(priority, "%s", localMsg.constData());
}

void exitApp(int signal){
    if(signal == SIGTERM){
        qInfo() << "Exiting...";
        QCoreApplication::exit(0);
    }
}

int run(int argc, char** argv){
    signal(SIGTERM, exitApp);

    if(!(argc > 1 && strcmp(argv[1], "--nodaemon") == 0))
        qInstallMessageHandler(syslogOutput);
    QLoggingCategory::setFilterRules(QStringLiteral("qt.bluetooth* = true"));

    qInfo() << "Starting...";

    QCoreApplication app(argc, argv);
    CelluloLocalRelayServer server;
    server.setListening(true);
    return app.exec();
}

int main(int argc, char** argv){
    if(argc > 1 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)){
        qInfo() << "Cellulo Robot Pool Daemon - Usage:";
        qInfo() << "    --help|-h:  Display this message";
        qInfo() << "    --nodaemon: Run in the foreground terminal instead of a daemon";
    }
    else if(argc > 1 && strcmp(argv[1], "--nodaemon") == 0)
        return run(argc, argv);
    else{

        //Fork off and die
        pid_t pid = fork();
        if(pid < 0)
            exit(EXIT_FAILURE);
        if(pid > 0)
            exit(EXIT_SUCCESS);

        //pid == 0 in the child, i.e the daemon

        openlog("cellulorobotpoold", LOG_CONS | LOG_NDELAY | LOG_PID, LOG_USER);

        //Files created have 777 permissions
        umask(0);

        pid_t sid = setsid();
        if(sid < 0)
            exit(EXIT_FAILURE);
        if((chdir("/")) < 0)
            exit(EXIT_FAILURE);

        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        return run(argc, argv);
    }
}
