cellulo-locomotion-demo
=======================

Demonstrates the locomotion capability of the Cellulo robots.

Tested with Qt 5.10.1 on:

- Ubuntu 17.10
- macOS 10.13.3 with Xcode 9.3
- Windows 10 (UWP x64 (MSVC 2017)) with Visual Studio 2017 Community (with `Universal Windows Platform development` and `C++ Universal Windows Platform tools`)
- Android 7.1.2 with Ubuntu 17.10 host with Android API 23, Android SDK Tools 25.2.5 and Android NDK r10e

Build
-----

1. Build and install [qml-cellulo](../../).
1. Load into Qt Creator, click `Build Project`.

Run
---

1. Print [assets/paper-dotted.pdf](assets/paper-dotted.pdf)
1. Click `Run` inside Qt Creator
1. Connect to a robot
1. Click `Go`
1. Robot should go to the designated pose, even after being kidnapped and put elsewhere; robot should light up in a random color every time it reaches the pose again
1. Play with the pose parameters
