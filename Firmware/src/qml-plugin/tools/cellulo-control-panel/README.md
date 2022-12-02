cellulo-control-panel
=====================

Tool to control and listen to all features of a single Cellulo robot.

Tested with Qt 5.10.0 on:

- Ubuntu 17.10
- macOS 10.13.3 with Xcode 9.3
- Windows 10 (UWP x64 (MSVC 2017)) with Visual Studio 2017 Community (with `Universal Windows Platform development` and `C++ Universal Windows Platform tools`)
- Android 7.1.2 with Ubuntu 17.10 host with Android API 23, Android SDK Tools 25.2.5 and Android NDK r10e (also available on the Play Store for direct download)

Linux & macOS build
-------------------

1. Build and install [qml-cellulo](../../).
1. Run the following:

    ```
    mkdir build && cd build
    qt-install-root/5.10.0/target-platform/bin/qmake ..
    make -j5
    sudo make install
    ```

Android build
-------------

1. Build and install [qml-cellulo](../../).
1. Load into Qt Creator, connect your device over USB, click `Run`.

Windows build
-------------

1. Build and install [qml-cellulo](../../).
1. Run the following in a `Developer Command Prompt for VS 2017` launched with administrator privileges (right click, select `Run as administrator`) while paying attention to slash directions, some of them are regular and some of them are backslashes:

    ```
	> "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64
	> mkdir build
	> cd build
	> qt-install-root\qt-version\target-platform\bin\qmake ..
    > nmake
	> nmake install
    > qt-install-root\qt-version\target-platform\bin\windeployqt --qmldir .. "C:/Program Files (x86)/Cellulo Control Panel/cellulo-control-panel.exe"
    > qt-install-root\qt-version\target-platform\bin\winrtrunner --profile appx --install "C:/Program Files (x86)/Cellulo Control Panel/cellulo-control-panel.exe"
    ```

This will install the control panel as an app to `C:\Program Files (x86)\Cellulo Control Panel\`. Alternatively, the app can also be loaded into Qt Creator, built and run from there.

Linux usage
-----------

Run `cellulo-control-panel` in the terminal.

macOS usage
-----------

Launch the `cellulo-control-panel` app from Launchpad.

Android usage
-------------

Click `Run` from within Qt Creator to install and launch or manually launch the app if it's already installed.

Windows usage
-------------

Launch `Cellulo Control Panel` from the Start Menu.

**Note:** It seems that for robots to appear in the scan, they must be paired manually beforehand from the Windows Bluetooth stack.
