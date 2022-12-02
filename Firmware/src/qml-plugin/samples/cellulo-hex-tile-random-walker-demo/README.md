cellulo-hex-tile-random-walker-demo
===================================

App that demonstrates hex tile functionality.

Tested with Qt 5.10.1 on:

- Ubuntu 18.04
- macOS 10.13.3 with Xcode 9.3
- Windows 10 (UWP x64 (MSVC 2017)) with Visual Studio 2017 Community (with `Universal Windows Platform development` and `C++ Universal Windows Platform tools`)
- Android 7.1.2 with Ubuntu 18.04 host with Android API 23, Android SDK Tools 25.2.5 and Android NDK r10e

Build
-----

1. Build and install [qml-cellulo](../../).
1. Load into Qt Creator, click `Build Project`.

Preparation
-----------

This demo runs on standard Cellulo hex tiles. These are regular hexagons with about 99 mm width (and about 114.32 mm
end-to-end) that each contain a unique area in the coordinate system. A4 sheets with 4 such tiles can be produced by the
`generate-hex-tile-quad-a4.py` script in [../../assets/hex-tile/](../../assets/hex-tile/). This script requires an `i`
coordinate and a `j` coordinate to generate an A4 sheet (don't use the same i,j twice in one application!). These sheets
can then be printed onto self-adhesive sheets and glued onto physical hexagons if desired. Obtain any desired number of
such tiles for this demo.

Run
---

1. Build your map with the tiles by juxtaposing them: Be careful that the marked corner of all tiles point in the same direction!
1. Launch the demo, connect to one or two robots.
1. Scan the map (`Autobuild` must be enabled) with one or two robots:
    1. The first tile is scanned automatically upon placing a robot onto a tile.
    1. The following tiles must be scanned by moving any robot from any known tile to the desired tile (must be neighbors) in a straight path without lifting.
    1. If any tile is scanned to be in a wrong place (move the robot around to be sure), it can be clicked and deleted from the appearing menu.
1. If the map is correct, `Save Map` can be used to save the map to a file which can be loaded later with `Load Map`.
1. Enabling `Walk?` should make the robot(s) move to a random tile each time.
