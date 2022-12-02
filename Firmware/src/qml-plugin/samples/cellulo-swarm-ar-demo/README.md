cellulo-swarm-ar-demo
=====================

Demonstrates augmented reality through `qml-ar` within a scalable Cellulo context, i.e unlimited number of robots.

Tested with Qt 5.11.0 on:

- Android 8.1.0 with Ubuntu 18.04 host with Android API 26, Android SDK Tools 25.2.5 and Android NDK r15c

The demo is only tested on Android as a mobile device is required for augmented reality.

Build
-----

1. Build and install [qml-cellulo](../../).
1. For pool/hub functionality, build and install [tools/cellulo-robot-pool](../../tools/cellulo-robot-pool/) and/or [tools/cellulo-robot-hub-gui](../../tools/cellulo-robot-hub-gui/). At least one is required. **Important note:** Robot pool does not work on Windows and a hub connection is mandatory.
1. Build and install [`qml-ar`](https://github.com/chili-epfl/qml-ar).
1. Load into Qt Creator, click `Build Project`.

Run
---

1. Print [assets/cellulo-empty-ar-sheet-coarse-dotted.pdf](assets/.pdf). The marker file is already added to the assets
under the name `cellulo-empty-ar-sheet-coarse-markers.json`. The activity can be run also on
`assets/cellulo-empty-ar-sheet-fine-dotted.pdf` but the camera needs to be much closer to the activity as the markers
are smaller. Also, the markers json file attribute of `ARComponent` must be changed accordingly to the
`cellulo-empty-ar-sheet-coarse-markers.json` file, but it is not added to the `qrc` assets by default. 1. Set up the
connection to the pool or a hub, connect to the desired number of robots. 1. There should be a sphere displayed on each
robot in the map in AR. The delta rotation of the robot should act as a "screw" to the sphere, raising or lowering the
individual sphere.
