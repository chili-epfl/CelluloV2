cellulo-motion-test
===================

Tests the locomotion quality of a Cellulo robot.

Tested with Qt 5.10.1 on:

- Ubuntu 18.04
- macOS 10.13.3 with Xcode 9.3
- Android 7.1.2 with Ubuntu 18.04 host with Android API 23, Android SDK Tools 25.2.5 and Android NDK r10e

Dependencies:

- Qt Charts

Build
-----

1. Install Qt Charts from the Maintenance Tool, which is not installed by default.
1. Build and install [qml-cellulo](../../).
1. Load into Qt Creator, click `Build Project`.

Run
---

1. Print [assets/paper-dotted.pdf](assets/paper-dotted.pdf)
1. Click `Run` inside Qt Creator
1. Connect to a robot
1. Click `Start`
1. Robot should travel from start to end at 12 orientations separated by 30 degrees, the motion quality is plotted on the right for each of these orientations
