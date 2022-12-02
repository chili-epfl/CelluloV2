TEMPLATE = app

linux:!android {
    message("Building for Linux")

    TARGET = cellulorobotpoold

    QT += core bluetooth quick

    SOURCES += src/main_linux.cpp

    INCLUDEPATH += \
        ../../../src/comm/ \
        ../../../src/comm/relay/ \
        ../../../include/

    LIBS += -L$$[QT_INSTALL_QML]/Cellulo/ -lcelluloplugin
    QMAKE_RPATHDIR += $$[QT_INSTALL_QML]/Cellulo/

    target.path = /usr/local/bin/
    INSTALLS += target
}

macx:!android {
    message("Building for macOS")

    TARGET = "Cellulo Robot Pool Daemon"

    QT += core bluetooth quick

    SOURCES += src/main_macos.cpp
    RESOURCES += qml_macos.qrc

    ICON = assets/cellulorobotpoold.icns
    QMAKE_INFO_PLIST = assets/Info.plist

    target.path = /Applications/
    INSTALLS += target
}

android{
    message("Building for Android")

    TARGET = cellulorobotpoold

    QT += core bluetooth quick androidextras network

    SOURCES += src/main_android.cpp
    RESOURCES += qml_android.qrc
    OTHER_FILES += \
        src/CelluloRobotPoolService.java
    android_java_sources.path = /src/ch/epfl/chili/cellulo/cellulorobotpoold
    android_java_sources.files = src/CelluloRobotPoolService.java

    INCLUDEPATH += \
        ../../../src/comm/ \
        ../../../src/comm/relay/ \
        ../../../include/
    LIBS += -L$$[QT_INSTALL_QML]/Cellulo/ -lcelluloplugin
    ANDROID_EXTRA_LIBS = \
        $$[QT_INSTALL_QML]/Cellulo/libcelluloplugin.so

    DISTFILES += \
        android/AndroidManifest.xml \
        android/gradle/wrapper/gradle-wrapper.jar \
        android/gradlew \
        android/res/values/libs.xml \
        android/build.gradle \
        android/gradle/wrapper/gradle-wrapper.properties \
        android/gradlew.bat
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
    target.path = /libs/armeabi-v7a
    export(target.path)
    INSTALLS += target android_java_sources
    export(INSTALLS)
}
