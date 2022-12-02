cellulo-haptics-demo
====================

Demonstrates some of the haptic capabilities of the Cellulo robot in rendering some surfaces.

Tested with Qt 5.10.1 on:

- Ubuntu 18.04
- macOS 10.13.3 with Xcode 9.3
- Windows 10 (UWP x64 (MSVC 2017)) with Visual Studio 2017 Community (with `Universal Windows Platform development` and `C++ Universal Windows Platform tools`)
- Android 8.1.0 with Ubuntu 18.04 host with Android API 26, Android SDK Tools 25.2.5 and Android NDK r15c

Build
-----

1. Build and install [qml-cellulo](../../).
1. Load into Qt Creator, click `Build Project`.

Run
---

1. Print [assets/map-step-1.pdf](assets/map-step-1.pdf)
1. Print [assets/map-step-2.pdf](assets/map-step-2.pdf) on top of the sheet that is just printed; make sure that the orientation is correct
1. Click `Run` inside Qt Creator
1. Connect to a robot
1. Feel the various areas that give appropriate haptic feedback; transitions from one area to the next should be smooth
