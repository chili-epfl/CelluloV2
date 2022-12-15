Cellulo robot firmware
======================

This repository contains the Cellulo robot firmware and tools. A new Cellulo robot must first have its Bluetooth module configured. Then, the firmware must be built and flashed to the microcontroller.

Building and flashing the PIC32MZ firmware
==========================================
What you need (available at [http://icchilisrv1.epfl.ch](http://icchilisrv1.epfl.ch)):

  - MPLAB X 2.35
  - xc 1.34
  - Harmony 1.03 (to install, go to Tools -> Plugins -> Downloaded -> Add Plugins and select `harmony-install-path/v1_03/utilities/mhc/com-microchip-mplab-modules-mhc.nbm`, then press Install)

Load the project into MPLAB X, connect the robot over ICSP and click the *Make and Program Device* button.

Configuring the ESP32 for bluetooth
=====================================
To configure the ESP32 module it is needed to connect to the ESP32 through the RX and TX connectors.

Once it is connected, it is possible to run the "BTConfiguration.ino" file provided in the github repository.

To change the name of the cellulo modify the following line with the desired name:

```
SerialBT.begin("cellulo-4126"); //Bluetooth device name
```

=========================================

If it is your first time working with Cellulo, you might need to install extra libraries libdots and qml-plugin.
Follow the guide in: https://c4science.ch/w/chili-epfl/cellulo/cellulo-software/cellulo-installation-linux-android/
