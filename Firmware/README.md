Cellulo robot firmware
======================

This repository contains the Cellulo robot firmware and tools. A new Cellulo robot must first have its Bluetooth module configured. Then, the firmware must be built and flashed to the microcontroller.

Configuring the RN42 Bluetooth module
=====================================

Upon powering up the robot, within 60 seconds, run the `rn42-autoconf.py` script under `scripts/` (`pybluez` should be installed). To do it manually, connect to it with a serial console over Bluetooth. Within 60 seconds of powerup, type

```
$$$
```

to enter command mode. The module should respond with `CMD<cr><lf>`. Then, type

```
SU,921K<cr>
```

The module should respond with `AOK<cr><lf>`. Then, type

```
ST,255<cr>
```

The module should respond with `AOK<cr><lf>`. Then, type

```
SW,0140<cr>
```

The module should respond with `AOK<cr><lf>`. Then, type

```
SJ,0600<cr>

```
The module should respond with `AOK<cr><lf>`. Then, type

```
SA,2<cr>
```

The module should respond with `AOK<cr><lf>`. Finally, reboot the module with

```
R,1<cr>
```

Now build and flash the firmware to the microcontroller.

Building and flashing the PIC32MZ firmware
==========================================
What you need (available at [http://icchilisrv1.epfl.ch](http://icchilisrv1.epfl.ch)):

  - MPLAB X 2.35
  - xc 1.34
  - Harmony 1.03 (to install, go to Tools -> Plugins -> Downloaded -> Add Plugins and select `harmony-install-path/v1_03/utilities/mhc/com-microchip-mplab-modules-mhc.nbm`, then press Install)

Load the project into MPLAB X, connect the robot over ICSP and click the *Make and Program Device* button.


=========================================

If it is your first time working with Cellulo, you might need to install extra libraries libdots and qml-plugin.
Follow the guide in: https://c4science.ch/w/chili-epfl/cellulo/cellulo-software/cellulo-installation-linux-android/
