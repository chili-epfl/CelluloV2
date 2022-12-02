TEMPLATE = app

QT += core qml quick bluetooth

macx{
    CONFIG -= app_bundle
}

SOURCES += \
    src/main.cpp \
    ../../src/util/svg/CelluloSVGUtil.cpp

HEADERS += \
    ../../src/util/svg/CelluloSVGUtil.h

INCLUDEPATH += \
    ../../src/util/svg/ \
    ../../include/

LIBS += -L$$[QT_INSTALL_QML]/Cellulo/ -lcelluloplugin
QMAKE_RPATHDIR += $$[QT_INSTALL_QML]/Cellulo/
macx{
    QMAKE_POST_LINK += install_name_tool -change libcelluloplugin.dylib @rpath/libcelluloplugin.dylib $$TARGET
}

unix{
    target.path = /usr/local/bin/
    INSTALLS += target
}
