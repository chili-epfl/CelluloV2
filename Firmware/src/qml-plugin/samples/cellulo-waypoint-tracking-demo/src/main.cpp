#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQmlContext>

int main(int argc, char *argv[]){
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///src/main.qml")));
    QObject *root = engine.rootObjects().first();
    return app.exec();
}
