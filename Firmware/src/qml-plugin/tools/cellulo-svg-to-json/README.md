cellulo-svg-to-json
===================

Command line tool to extract Cellulo zones from SVG files designed in vector graphic editors, such as Inkscape.

Tested with Qt 5.10.1 on:

  - Ubuntu 17.10
  - macOS 10.13.3 with Xcode 9.3

Doesn't work (and is not needed) on Android. Doesn't work either on Windows but `cellulo-svg-to-json-gui` may be used instead.

Build
-----

1. Build and install [qml-cellulo](../../).
1. Run the following:

    ```
    mkdir build && cd build
    qt-install-root/qt-version/target-platform/bin/qmake ..
    make -j5 install
    ```

Run
---

```
cellulo-svg-to-json --help
```
