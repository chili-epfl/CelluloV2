TEMPLATE = app

QT += qml quick bluetooth

SOURCES += src/main.cpp

RESOURCES += qml.qrc

linux:!android {
    message("Building for Linux")

    TARGET = cellulo-control-panel

    target.path = /usr/local/bin/
    INSTALLS += target
}

macx:!android {
    message("Building for macOS")

    TARGET = "Cellulo Control Panel"

    ICON = assets/cellulo-control-panel.icns
    QMAKE_INFO_PLIST = assets/Info.plist

    target.path = /Applications/
    INSTALLS += target
}

android {
    message("Building for Android")

    target.path = /libs/armeabi-v7a
    export(target.path)
    INSTALLS += target
    export(INSTALLS)

    ANDROID_EXTRA_LIBS = \
        $$[QT_INSTALL_QML]/Cellulo/libcelluloplugin.so

    ANDROID_PERMISSIONS += \
        android.permission.BLUETOOTH
}

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

winrt {
    message("Building for Windows")

    target.path = "C:/Program Files (x86)/Cellulo Control Panel/"
    INSTALLS += target

    WINRT_MANIFEST.publisher = "EPFL CHILI"
    WINRT_MANIFEST.name = "Cellulo Control Panel"
    WINRT_MANIFEST.description = "Controls all aspects of a single Cellulo robot."
    WINRT_MANIFEST.background = white
    WINRT_MANIFEST.logo_large = assets/winrt-logo-large.png
    WINRT_MANIFEST.logo_store = assets/winrt-logo-store.png
    WINRT_MANIFEST.logo_44x44 = assets/winrt-logo-44-44.png
    WINRT_MANIFEST.logo_620x300 = assets/winrt-logo-620-300.png
}
