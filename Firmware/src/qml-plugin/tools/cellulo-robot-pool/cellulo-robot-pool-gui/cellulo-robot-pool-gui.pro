TEMPLATE = app

QT += qml quick bluetooth

SOURCES += src/main.cpp

RESOURCES += qml.qrc

linux:!android {
    message("Building for Linux")

    TARGET = cellulo-robot-pool-gui

    target.path = /usr/local/bin/
    INSTALLS += target
}

macx:!android {
    message("Building for macOS")

    TARGET = "Cellulo Robot Pool GUI"

    ICON = assets/cellulo-robot-pool-gui.icns
    QMAKE_INFO_PLIST = assets/Info.plist

    target.path = /Applications/
    INSTALLS += target
}

android{
    message(Building for Android)

    TARGET = cellulo-robot-pool-gui

    target.path = /libs/armeabi-v7a
    export(target.path)
    INSTALLS += target
    export(INSTALLS)

    DISTFILES += \
        android/AndroidManifest.xml \
        android/gradle/wrapper/gradle-wrapper.jar \
        android/gradlew \
        android/res/values/libs.xml \
        android/build.gradle \
        android/gradle/wrapper/gradle-wrapper.properties \
        android/gradlew.bat

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

    INCLUDEPATH += \
        ../../../src/comm/ \
        ../../../src/comm/relay/ \
        ../../../include/

    LIBS += -L$$[QT_INSTALL_QML]/Cellulo/ -lcelluloplugin

    ANDROID_EXTRA_LIBS = \
        $$[QT_INSTALL_QML]/Cellulo/libcelluloplugin.so
}


