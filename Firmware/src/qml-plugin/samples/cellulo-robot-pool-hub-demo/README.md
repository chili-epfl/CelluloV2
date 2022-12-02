cellulo-robot-pool-hub-demo
===========================

Demonstrates swarm formations with the Cellulo robots. Also demonstrates the use of `cellulo-robot-pool` (on the same
machine) and `cellulo-robot-hub` (through a remote machine, such as a Raspberry PI) to easily connect to many robots
that can be used in any activity without disconnecting.

Tested with Qt 5.11.0 on:

- Ubuntu 18.04
- macOS 10.13.3 with Xcode 9.3
- Windows 10 (UWP x64 (MSVC 2017)) with Visual Studio 2017 Community (with `Universal Windows Platform development` and `C++ Universal Windows Platform tools`)
- Android 7.1.2 with Ubuntu 18.04 host with Android API 23, Android SDK Tools 25.2.5 and Android NDK r10e

**Note on the pool functionality:** As the pool functionality requires `cellulorobotpoold` running locally, it will
not work on Windows since WinRT sandboxing prevents multiple apps from communicating with each other. On Windows, use
the Raspberry PI with `cellulorobothubd` installed. See
[https://c4science.ch/w/chili-epfl/cellulo/cellulo-software/cellulo-installation-rpi/](https://c4science.ch/w/chili-epfl/cellulo/cellulo-software/cellulo-installation-rpi/)
for more information on obtaining one.

Build
-----

1. Build and install [qml-cellulo](../../).
1. For pool functionality, build and install [tools/cellulo-robot-pool](../../tools/cellulo-robot-pool/). The build and
install heavily depends on your platform, see [tools/cellulo-robot-pool/README.md](../../tools/cellulo-robot-pool/README.md).
1. For hub functionality, obtain a remote machine that runs a `cellulorobothubd`, such as a Raspberry PI. See
[https://c4science.ch/w/chili-epfl/cellulo/cellulo-software/cellulo-installation-rpi/](https://c4science.ch/w/chili-epfl/cellulo/cellulo-software/cellulo-installation-rpi/)
for more information on obtaining one. Then, build and install [tools/cellulo-robot-hub-gui](../../tools/cellulo-robot-hub-gui/) on the main machine.
1. Load into Qt Creator, click `Build Project`.

Run [Pool functionality]
------------------------

1. Print [assets/map-dotted.pdf](assets/map-dotted.pdf); can be run on a smaller playground too, but the larger the better for manipulation comfort
1. Plug in external Bluetooth dongles,  at least one dongle for each 5 robots must be present; so for 15 robots, at least 2 external dongles are needed (in addition to the internal adapter). **Note that the only platform that supports more than one dongle is Linux through the Bluez stack.**
1. Depending on your platform, follow one of these steps:
  - *Linux* - Run `sudo cellulo-robot-pool-gui` to launch the robot pool daemon and its GUI. The daemon can be managed from the GUI if desired, but should work as it is.
  - *macOS* - Launch the **Cellulo Robot Pool Daemon** app. You can minimize it to the dock without closing it. Then, launch the **Cellulo Robot Pool GUI** app.
  - *Android* - Run the **Cellulo Robot Pool Service** app and click `Start service`. When connected (might take few seconds), exit the app. Then, launch the **Cellulo Robot Pool GUI** app (it might also take a few seconds to connect to the service).
1. Click `Scan` to scan the robots around (done locally on the main machine). **Note for WinRT:** For robots to appear in the scan, they must first be scanned and paired from the OS Bluetooth scanner.
1. Click `+` next to the discovered robots to add them to the pool or click `Add all above` to add all found robots
1. [Only available on *Linux*] If more than 5 robots are desired, click `Equally distribute local adapters` or select local adapters manually (second drop-down box on each list item)
1. Click `Connect to all`
1. Once all robots are connected, close the Robot Pool GUI.
1. Load `cellulo-robot-pool-hub-demo` into Qt Creator, configure for the desired platform and click `Run`
1. Check `Go`
1. Robots should come into a lattice formation, but you probably need to put the robots in their correct place manually at first since there is no pathfinding algorithm present
1. Touch any button of a robot to interact with it:
    - Move one robot around; the rest of the lattice should follow
    - Move two robots to do the "pinch-zoom" gesture on the whole lattice

Run [Hub functionality]
------------------------

1. Print [assets/map-dotted.pdf](assets/map-dotted.pdf); can be run on a smaller playground too, but the larger the better for manipulation comfort.
1. Prepare a machine (such as a Raspberry PI) that runs `cellulorobothubd`. See
[https://c4science.ch/w/chili-epfl/cellulo/cellulo-software/cellulo-installation-rpi/](https://c4science.ch/w/chili-epfl/cellulo/cellulo-software/cellulo-installation-rpi/)
for more information on obtaining one.
1. Plug in external Bluetooth dongles on this remote machine, at least one dongle for each 5 robots must be present; so for 15 robots, at least 2 external dongles are needed (in addition to the internal adapter).
1. Connect to this machine through a TCP channel such as WiFi or ethernet.
1. Launch the Robot Hub GUI and enter this machine's IP (if using the default Raspberry PI setup, `192.168.4.1` for WiFi and `192.168.2.1` for ethernet) and the port that `cellulorobothubd` is listening on (2556 by default) to the corresponding boxes. Connection should be pretty much immediately established.
1. Click `Scan` to scan the robots around (done locally on the main machine). **Note for WinRT:** For robots to appear in the scan, they must first be scanned and paired from the OS Bluetooth scanner.
1. Click `+` next to the discovered robots to add them to the pool or click `Add all above` to add all found robots
1. If more than 5 robots are desired, click `Equally distribute local adapters` or select local adapters manually (second drop-down box on each list item)
1. Click `Connect to all`
1. Once all robots are connected, close the Robot Hub GUI.
1. Load `cellulo-robot-pool-hub-demo` into Qt Creator, configure for the desired platform and click `Run`
1. Select "Hub" and enter the same IP and port as you did in the Hub GUI
1. Check `Go`
1. Robots should come into a lattice formation, but you probably need to put the robots in their correct place manually at first since there is no pathfinding algorithm present
1. Touch any button of a robot to interact with it:
    - Move one robot around; the rest of the lattice should follow
    - Move two robots to do the "pinch-zoom" gesture on the whole lattice
