cellulo-qml-plugin
==================

This is a qml module for communicating with Cellulo robots over Bluetooth. It was tested:

  - With Qt 5.10.0
  - On Ubuntu 17.10
  - On macOS 10.13.3 with Xcode 9.3
  - On Windows 10 (UWP x64 (MSVC 2017)) with Visual Studio 2017 Community (with `Universal Windows Platform development` and `C++ Universal Windows Platform tools`)
  - On Android 7.1.2 with Ubuntu 17.10 host with Android API 23, Android SDK Tools 26.1.1 and Android NDK r15c

Below are the USB Bluetooth dongles that were confirmed to work with the Cellulo robots under Ubuntu 17.10 with BlueZ 5.46:

  - Asus USB-BT400, reports a `Broadcom BCM20702A0` to be present, bought from https://www.conrad.ch/ce/fr/product/1090718/
  - Vivanco 30447, reports a `Broadcom BCM20702A0` to be present, bought from https://www.conrad.ch/ce/fr/product/673433/
  - No-brand pcDuino USB dongle, reports a `Cambridge Silicon Radio, Ltd` to be present, bought from https://www.conrad.ch/ce/fr/product/1233685/
  - Conrad 448876, reports a `Cambridge Silicon Radio, Ltd` to be present, bought from https://www.conrad.ch/ce/fr/product/448876/
  - Digitus DN-30210-1, reports a `Cambridge Silicon Radio, Ltd` to be present, bought from https://www.conrad.ch/ce/fr/product/395775/
  - Renkforce Bluetooth® 4.0 stick, reports a `Cambridge Silicon Radio, Ltd` to be present, bought from https://www.conrad.ch/ce/fr/product/1483951/
  - D-Link DBT-120, reports a `Cambridge Silicon Radio, Ltd` to be present, bought from ????

Tools are under [tools/](tools/), samples are under [samples/](samples/). See their README's for instructions.

See [doc/index.html](doc/index.html) for the API.

Dependencies
-------------

  - [qml-bluetooth-extras](https://github.com/chili-epfl/qml-bluetooth-extras): Must be installed to obtain the list of local Bluetooth adapters, see the repository for installation instructions.
  - [qml-extra-data-structures](https://github.com/chili-epfl/qml-extra-data-structures): Data structures missing from the QML world, see the repository for installation instructions.
  - [qml-cache](https://github.com/chili-epfl/qml-cache): Persistent app-specific key-value store, see the repository for installation instructions.
  - [qml-logger](https://github.com/chili-epfl/qml-logger): File logging utilities that we commonly used for data collection in experiments, see the repository for installation instructions.

Desktop build (Linux & macOS)
-----------------------------

```
    $ mkdir build && cd build
    $ qt-install-root/qt-version/target-platform/bin/qmake ..
    $ make -j5 install
```

This will install the QML plugin inside the Qt sysroot, so it must be writable by you. **Be aware that this is not a sandboxed installation.**

Desktop build (Windows)
-----------------------

Run the following in the `Developer Command Prompt for VS 2017`:

```
    > "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64
    > mkdir build
    > cd build
    > qt-install-root\qt-version\target-platform\bin\qmake ..
    > nmake
    > nmake install
```

This will install the QML plugin inside the Qt sysroot, so it must be writable by you. **Be aware that this is not a sandboxed installation.**

Android build
-------------

```
    $ export ANDROID_NDK_ROOT=/path-to-android-ndk/
    $ mkdir build-android && cd build-android
    $ qt-install-root/qt-version/android_armv7/bin/qmake ..
    $ make -j5 install
```

This will install the QML plugin inside the Qt sysroot, so it must be writable by you. **Be aware that this is not a sandboxed installation.**

Build & run unit tests
----------------------

```
    $ mkdir build-test && cd build-test
    $ qt-install-root/qt-version/target-platform/bin/qmake .. "CONFIG += test"
    $ make -j5
    $ ./celluloplugintests
```

Build documentation
-------------------

Install dependencies:

```
    $ apt install doxygen doxyqml
```

Then, generate the documentation:

```
    $ doxygen
```
