cellulo-bezier-zones-demo
=========================

Demonstrates more complicated PolyBézier zones that can be used with the Cellulo robots.

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

1. Print [assets/page-dotted.pdf](assets/page-dotted.pdf)
1. Click `Run` inside Qt Creator
1. Connect to a robot
1. Move robot on the map and see:
    - Distance to the PolyBézier curve being calculated
    - Tangent and normal vectors to the curve on the point closest to the robot being calculated, via the closest point's parameter `t`
    - Inside of the PolyBézier curve making the robot red
    - Border of the PolyBézier curve making the robot blue
    - Intersection of border and inside of the PolyBézier curve making the robot violet
