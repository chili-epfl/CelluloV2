cellulo-waypoint-tracking-demo
==============================

Demonstrates the use of advanced curve trackers to track a list of future waypoints.

Tested with Qt 5.10.1 on:

- Ubuntu 18.04
- macOS 10.13.3 with Xcode 9.3
- Windows 10 (UWP x64 (MSVC 2017)) with Visual Studio 2017 Community (with `Universal Windows Platform development` and `C++ Universal Windows Platform tools`)
- Android 7.1.2 with Ubuntu 17.10 host with Android API 23, Android SDK Tools 25.2.5 and Android NDK r10e

Build
-----

1. Build and install [qml-cellulo](../../).
1. Load into Qt Creator, click `Build Project`.

Run
---

1. Print [assets/page-dotted.pdf](assets/page-dotted.pdf)
1. Click `Run` inside Qt Creator
1. Connect to a robot, put the robot on the page
1. Start tracking
1. Add waypoints to the curve by clicking on the playground, robot will track them in order
