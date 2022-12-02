#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtBluetooth>

int main(int argc, char *argv[]){
    QLoggingCategory::setFilterRules(QStringLiteral("qt.bluetooth* = true"));
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///src/main.qml")));
    return app.exec();
}
