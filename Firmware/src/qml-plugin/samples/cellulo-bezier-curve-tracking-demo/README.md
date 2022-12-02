cellulo-bezier-curve-tracking-demo
==================================

Demonstrates the use of advanced curve trackers.

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
1. Connect to a robot
1. Try the available trackers:
  1. **Constant velocity tracker**: Tracks the curve with the given constant velocity
  1. **Constant acceleration tracker**: Tracks the curve with the given constant velocity, accelerates and decelerates up/down to speed with the given finite acceleration
  1. **Adaptive velocity tracker**: Tracks the curve with the given maximum velocity around straight parts of the path, decreases this velocity down to the minimum around the parts with high curvature
  1. **Profiled velocity tracker**: Tracks the curve with the given velocity profile; this is a list of velocities to adopt within equally spaced points on the curve
1. Try the common properties of trackers:
  1. When launched, go to start or start at the closest point
  1. When done, stop tracking; this may not be desirable if segments are expected to be added to the curve
  1. Clean up curve as tracked: Remove finished segments from the curve, may be desirable for performance reasons
1. Try to add segments to the curve by clicking on the playground
