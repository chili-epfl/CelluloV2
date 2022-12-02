cellulo-robot-hub-gui
=====================

A tool that provides full control over a remote `cellulorobothubd` (see [cellulorobothubd](../cellulorobothubd/)) over
TCP:

    - *Linux* - Launch `cellulo-robot-hub-gui` from the terminal.
    - *macOS* - Launch as an app (called `Cellulo Robot Hub GUI`).
    - *Android* - Launch as an app (called `Cellulo Robot Hub GUI`). This app is available on the Play Store for download.
    - *Windows* - Launch as an app (called `Cellulo Robot Hub GUI`).

While the remote `cellulorobothubd` is alive, a `CelluloRobotHubClient` in a Cellulo application can connect to it and
communicate with its already connected robots.

Tested with Qt 5.11.0 on:

  - Ubuntu 18.04
  - macOS 10.13.3 with Xcode 9.4.1
  - Android 7.1.2 with Ubuntu 18.04 host with Android API 23, Android SDK Tools 25.2.5 and Android NDK r10e (both apps also available on the Play Store)
  - Windows 10 (UWP x64 (MSVC 2017)) with Visual Studio 2017 Community (with `Universal Windows Platform development` and `C++ Universal Windows Platform tools`)

Linux & macOS build
-------------------

Regardless of the platform:

1. Build and install [qml-cellulo](../../).
1. Run the following:

    ```
    mkdir build && cd build
    qt-install-root/qt-version/target-platform/bin/qmake ..
    make -j5
    sudo make install
    ```

Android build
-------------

Get the app from the Play Store or:

1. Build and install [qml-cellulo](../../).
1. Load `cellulo-robot-hub-gui` into Qt Creator, connect your device over USB, click `Run`.

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
    > qt-install-root\qt-version\target-platform\bin\windeployqt --qmldir .. "C:/Program Files (x86)/Cellulo Robot Hub GUI/cellulo-robot-hub-gui.exe"
    > qt-install-root\qt-version\target-platform\bin\winrtrunner --profile appx --install "C:/Program Files (x86)/Cellulo Robot Hub GUI/cellulo-robot-hub-gui.exe"
    ```

This will install the app to `C:\Program Files (x86)\Cellulo Robot Hub GUI\`. Alternatively, the app can also be loaded into Qt Creator, built and run from there.

Linux usage
-----------

Start a remote `cellulorobothubd` and connect to it via a TCP channel (see [cellulorobothubd](../cellulorobothubd/)).
Run `cellulo-robot-hub-gui` to launch the hub, set the IP address and port accordingly and wait for connection to the
server. "Scan for robots" panel can be used to scan and add available robots to the pool (done locally, the machine
running the hub must have Bluetooth functionality). Once added, they can be connected to using the "Robots on server"
panel. Once all desired robots are added and connected to, `cellulo-robot-hub-gui` should be closed. Then, these robots
in the hub can be used in an application through `CelluloRobotHubClient`, see
[samples/cellulo-robot-pool-hub-demo/](../../samples/cellulo-robot-pool-hub-demo/) for an example.

macOS usage
-----------

Start a remote `cellulorobothubd` and connect to it via a TCP channel (see [cellulorobothubd](../cellulorobothubd/)).
Launch the `Cellulo Robot Hub GUI` app, set the IP address and port accordingly and wait for connection to the server.
"Scan for robots" panel can be used to scan and add available robots to the pool (done locally, the machine
running the hub must have Bluetooth functionality). Once added, they can be connected to using the "Robots on server"
panel. Once all desired robots are added and connected to, `cellulo-robot-hub-gui` should be closed. Then, these robots
in the hub can be used in an application through `CelluloRobotHubClient`, see
[samples/cellulo-robot-pool-hub-demo/](../../samples/cellulo-robot-pool-hub-demo/) for an example.

Android usage
-------------

Start a remote `cellulorobothubd` and connect to it via a TCP channel (see [cellulorobothubd](../cellulorobothubd/)).

**Note on ethernet connection:** Starting with Android 6 (Marshmallow), USB ethernet adapters are enumerated and can be
used to obtain an IP. A converter cable can be used to plug into the USB-C/micro-USB port on the device. However, the
OS does not even ask for an IP over an ethernet connection which does not provide internet if there is another
connection that provides internet, such as GSM or WiFi. Since the device that provides `cellulorobothubd` will likely
not provide internet, these connections that do provide internet must be closed.

Launch the `Cellulo Robot Hub GUI` app, set the IP address and port accordingly and wait for connection to the server.
"Scan for robots" panel can be used to scan and add available robots to the pool (done locally, the machine
running the hub must have Bluetooth functionality). Once added, they can be connected to using the "Robots on server"
panel. Once all desired robots are added and connected to, `cellulo-robot-hub-gui` should be closed. Then, these robots
in the hub can be used in an application through `CelluloRobotHubClient`, see
[samples/cellulo-robot-pool-hub-demo/](../../samples/cellulo-robot-pool-hub-demo/) for an example.

Windows usage
-------------

Start a remote `cellulorobothubd` and connect to it via a TCP channel (see [cellulorobothubd](../cellulorobothubd/)).
Launch the `Cellulo Robot Hub GUI` app, set the IP address and port accordingly and wait for connection to the server.
"Scan for robots" panel can be used to scan and add available robots to the pool (done locally, the machine
running the hub must have Bluetooth functionality). Once added, they can be connected to using the "Robots on server"
panel. Once all desired robots are added and connected to, `cellulo-robot-hub-gui` should be closed. Then, these robots
in the hub can be used in an application through `CelluloRobotHubClient`, see
[samples/cellulo-robot-pool-hub-demo/](../../samples/cellulo-robot-pool-hub-demo/) for an example.
