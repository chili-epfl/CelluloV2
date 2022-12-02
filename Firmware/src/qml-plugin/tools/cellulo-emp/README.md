cellulo-emp
===========

Tool to scan and mass reset/shutdown all Cellulo robots (bit buggy and may crash at the moment).

Tested with Qt 5.10.1 on:

- Ubuntu 17.10
- macOS 10.13.3 with Xcode 9.3
- Windows 10 (UWP x64 (MSVC 2017)) with Visual Studio 2017 Community (with `Universal Windows Platform development` and `C++ Universal Windows Platform tools`)
- Android 7.1.2 with Ubuntu 17.10 host with Android API 23, Android SDK Tools 25.2.5 and Android NDK r10e (also available on the Play Store for direct download)

**Note for Windows:** It seems that for robots to appear in the scan, they must be paired manually beforehand from the Windows Bluetooth stack.

Build
-----

1. Build and install [qml-cellulo](../../).
1. Load into Qt Creator, click `Build Project`.

Run
---

1. Click `Run` inside Qt Creator
