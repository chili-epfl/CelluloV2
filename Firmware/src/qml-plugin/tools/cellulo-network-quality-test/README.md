cellulo-network-quality-test
============================

A tool that quantifies network connection quality over a robot pool or a robot hub, in a typical control scenario with a
given control loop frequency where all robots are simultaneously controlled in real time. The point is to test
connection quality to a large number of robots and find out which control frequency (i.e period) leads to an acceptable
control.

Tested with Qt 5.11.0 on:

  - Ubuntu 18.04
  - macOS 10.13.3 with Xcode 9.4.1
  - Android 7.1.2 with Ubuntu 18.04 host with Android API 23, Android SDK Tools 25.2.5 and Android NDK r10e (app also available on the Play Store)
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
    > qt-install-root\qt-version\target-platform\bin\windeployqt --qmldir .. "C:/Program Files (x86)/Cellulo Network Quality Test/cellulo-network-quality-test.exe"
    > qt-install-root\qt-version\target-platform\bin\winrtrunner --profile appx --install "C:/Program Files (x86)/Cellulo Network Quality Test/cellulo-network-quality-test.exe"
    ```

This will install the app to `C:\Program Files (x86)\Cellulo Network Quality Test\`. Alternatively, the app can also be loaded into Qt Creator, built and run from there.

Usage
-----

Either start a remote `cellulorobothubd` (see [cellulorobothubd](../cellulorobothubd/)) or a `cellulorobotpoold` (see
[cellulorobotpoold](../cellulo-robot-pool/cellulorobotpoold/)). Connect to it via a `Cellulo Robot Hub GUI` (see
[cellulo-robot-hub-gui](../cellulo-robot-hub-gui/)) or `Cellulo Robot Pool GUI` (see
[cellulo-robot-pool-gui](../cellulo-robot-pool-gui/)), add and connect to the desired number of robots, close the GUI.

Launch `Cellulo Network Quality Test` (`cellulo-network-quality-test` in the terminal in Linux) and pick the pool or the
hub connection. Place all robots somewhere on a dotted sheet and make sure they can rotate freely. Set the desired
control period (for 20 robots, a robust value seems to be ~170 ms, fewer robots should allow for faster control), check
whether color control should be enabled (simulates the scenario where the pose and the color are both controlled in real
time) and check `Go!`. All robots should rotate back and forth, in place. Observe the control quality visually, i.e
whether there are any clear drops in communication leading to some robots diverging in position. When `Go!` is checked
and unchecked, the motion should start and stop near immediately on all robots. The actual overall delay values can be
observed with the provided chart and measurements.
