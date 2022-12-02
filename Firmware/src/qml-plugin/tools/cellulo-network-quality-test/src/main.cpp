#include <QApplication>
#include <QQmlApplicationEngine>

int main(int argc, char* argv[]){
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///src/main.qml")));
    return app.exec();
}
