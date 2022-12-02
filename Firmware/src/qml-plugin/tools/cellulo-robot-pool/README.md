cellulo-robot-pool
==================

A suite of tools that provide a server that stays alive and connected to the Cellulo robots:

  - `cellulorobotpoold`: Daemon that encapsulates a `CelluloLocalRelayServer` where the Bluetooth sockets to Cellulo robots stay connected.
    - *Linux* - Launch as daemon (`sudo start-stop-daemon [--start|--stop] --exec /usr/local/bin/cellulorobotpoold`) or manage from `cellulo-robot-pool-gui`. All messages coming from the daemon are dumped via `syslog`. This is found in `/var/log/syslog` under Ubuntu.
    - *macOS* - Must be launched manually as an app (named `Cellulo Robot Pool Daemon`) and must be left running. All messages coming from the daemon are dumped via `syslog`. This is found in `/var/log/system.log` under macOS.
    - *Android* - Must be launched manually as an app (named `Cellulo Robot Pool Service`) that can start the daemon as a service. Then, the app must be closed. All messages coming from the daemon are dumped via logcat. This app is available on the Play Store for download.
  - `cellulo-robot-pool-gui`: controls `cellulorobotpoold` to add or remove robots as well as connect to or disconnect from them.
    - *Linux* - Launch from the terminal **with root privileges**. Can start and stop `cellulorobotpoold` as well.
    - *macOS* - Launch as an app (called `Cellulo Robot Pool GUI`). Cannot start and stop `Cellulo Robot Pool Daemon`, it must be launched manually as an app and must be left running.
    - *Android* - Launch as an app (called `Cellulo Robot Pool GUI`). Cannot start and stop `Cellulo Robot Pool Service` which must be launched manually as an app, see above. This app is available on the Play Store for download.

While `cellulorobotpoold` is alive, a `CelluloRobotPoolClient` in a Cellulo application can connect to the server and
communicate with its already connected robots.

Tested with Qt 5.11.0 on:

  - Ubuntu 18.04
  - macOS 10.13.3 with Xcode 9.4.1
  - Android 8.1.0 with Ubuntu 18.04 host with Android API 26, Android SDK Tools 25.2.5 and Android NDK r15c (both apps also available on the Play Store)

**Does not work on Windows** due to WinRT sandboxing preventing multiple apps from communicating with each other. This prevents any sort of daemon from being built within the WinRT framework. For Windows, see [cellulo-robot-hub-gui](../cellulo-robot-hub-gui/).

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

Get both apps from the Play Store or:

1. Build and install [qml-cellulo](../../).
1. Load `cellulorobotpoold` individually into Qt Creator, connect your device over USB, click `Run`.
  - **Note:** `build.gradle` must contain `dependencies { compile 'com.android.support:support-v4:26.1.0' }` and `allprojects { repositories { maven { url "https://maven.google.com" }}}` which is not included by default by QtCreator.
1. Load `cellulo-robot-pool-gui` individually into Qt Creator, connect your device over USB, click `Run`.

**Important note:** Does not work with Qt below 5.10.1 as it contains a crucial fix for Android services to work.

Linux usage
-----------

Run `sudo cellulo-robot-pool-gui` to launch the pool control panel. At startup, `cellulorobotpoold` will be started
automatically, which can be manually stopped or started as well from the "Server controls" panel. "Scan for robots"
panel can be used to scan and add available robots to the pool. Once added, they can be connected to using the "Robots
on server" panel. Once all desired robots are added and connected to, `cellulo-robot-pool-gui` can be closed. Then,
these robots in the pool can be used in an application through `CelluloRobotPoolClient`, see
[samples/cellulo-robot-pool-hub-demo/](../../samples/cellulo-robot-pool-hub-demo/) for an example.

**Important note:** It is advised to run `cellulorobotpoold` with root privileges (`sudo cellulo-robot-pool-gui` will
ensure that) for much faster connection to many robots over multiple adapters. It can also be run with user privileges
but the connections will possibly be established slower.

macOS usage
-----------

First, launch the `Cellulo Robot Pool Daemon` app. Do not close it. Then launch the `Cellulo Robot Pool GUI` app to launch the pool
control panel. "Scan for robots" panel can be used to scan and add available robots to the pool. Once added, they can be
connected to using the "Robots on server" panel. Once all desired robots are added and connected to,
`Cellulo Robot Pool GUI` can be closed. Then, these robots in the pool can be used in an application through
`CelluloRobotPoolClient`, see [samples/cellulo-robot-pool-hub-demo/](../../samples/cellulo-robot-pool-hub-demo/) for an example.

**Important note**: Multiple adapters are not supported on macOS, only the internal one is available. This will limit
the number of connectable robots certainly to 7 and practically to around 5. If you'd like to connect to more robots on
macOS, see [cellulo-robot-hub-gui](../cellulo-robot-hub-gui/).

Android usage
-------------

First, launch the `Cellulo Robot Pool Service` app. Click `Start service`. When connected (might take few seconds), exit the app. Then, launch the `Cellulo Robot Pool GUI` app (it might also take a few seconds to connect to the service). "Scan for robots" panel can be used to scan and add available robots to the pool. Once added, they can be
connected to using the "Robots on server" panel. Once all desired robots are added and connected to,
`Cellulo Robot Pool GUI` can be closed. Then, these robots in the pool can be used in an application through
`CelluloRobotPoolClient`, see [samples/cellulo-robot-pool-hub-demo/](../../samples/cellulo-robot-pool-hub-demo/) for an example.

**Important note**: Multiple adapters are not supported on Android, only the internal one is available. This will limit
the number of connectable robots certainly to 7 and practically to around 5. If you'd like to connect to more robots on
Android, see [cellulo-robot-hub-gui](../cellulo-robot-hub-gui/).
