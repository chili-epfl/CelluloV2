#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtBluetooth>

#include <syslog.h>
#include <stdarg.h>

void syslogOutput(QtMsgType type, QMessageLogContext const& context, QString const& msg){
    Q_UNUSED(context);

    QByteArray localMsg = msg.toLocal8Bit();

    int priority = LOG_NOTICE; //LOG_INFO; //macOS doesn't dump logs lower than LOG_NOTICE
    switch(type){
    case QtDebugMsg:
        priority = LOG_NOTICE; //LOG_INFO; //macOS doesn't dump logs lower than LOG_NOTICE
        break;

    case QtInfoMsg:
        priority = LOG_NOTICE; //LOG_INFO; //macOS doesn't dump logs lower than LOG_NOTICE
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

int main(int argc, char *argv[]){
    openlog("Cellulo Robot Pool Daemon", LOG_CONS | LOG_NDELAY | LOG_PID, LOG_USER);
    qInstallMessageHandler(syslogOutput);
    QLoggingCategory::setFilterRules(QStringLiteral("qt.bluetooth* = true"));
    qInfo() << "Starting...";

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///src/main_macos.qml")));
    return app.exec();
}
