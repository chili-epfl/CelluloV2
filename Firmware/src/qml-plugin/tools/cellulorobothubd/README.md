cellulorobothubd
================

A daemon that encapsulates a `CelluloTcpRelayServer` where the Bluetooth sockets to Cellulo robots stay connected.
Together with [cellulo-robot-hub-gui](../cellulo-robot-hub-gui/), `cellulorobothubd` provides a server that stays alive
and exports the robot connections over a TCP connection. In other words, while `cellulorobothubd` is alive, a
`CelluloRobotHubClient` in a Cellulo application can connect to the hub and communicate with its already connected
robots.

Tested with Qt 5.11.0 on:

  - Raspbian Stretch on a Raspberry PI 3 Model B

`cellulorobotpoold` only works on Linux due to it being the only platform that support multiple Bluetooth adapters.
Furthermore, it is specifically designed to be run on a lighter machine such as a Raspberry PI.

The installation ensures that it is launched at boot as a service. All messages coming from the daemon are dumped via
`syslog`. This is found in `/var/log/syslog` under Raspbian.

Build
-----

1. Install `libbluetooth-dev`.
1. Build and install [qml-cellulo](../../).
1. Run the following:

    ```
    $ mkdir build && cd build
    $ qt-install-root/qt-version/target-platform/bin/qmake ..
    $ make -j5
    $ sudo make install
    ```

Usage
-----

`cellulorobothubd` is launched as a service at boot. To manually start/stop/restart it (should not be normally necessary):

```
$ sudo service cellulorobothubd [start|stop|restart]
```

Connect the desired number of USB Bluetooth adapters on the device running `cellulorobothubd`.

**Warning:** Connecting directly on the USB ports next to each other will place the antennae too close to each other,
causing interference (or possibly some kind of coupling). Instead, plug these to your RPi through USB extension cables,
making sure that each adapter is farther than a few tens of centimeters with respect to each other.

With your desired device that has `Cellulo Robot Hub GUI` installed (see
[cellulo-robot-hub-gui](tools/cellulo-robot-hub-gui/)), connect to the device running `cellulorobothubd` over some TCP
channel. This could be WiFi (not recommended for high bandwidth applications e.g when connected robots are constantly
communicating with the app) or ethernet or some other channel that is fast enough.

Run `Cellulo Robot Hub GUI` to launch the hub control panel, select the address and port of the connection to the device
that runs `cellulorobothubd` (could be `192.168.2.1` for ethernet and `192.168.4.1` for WiFi) and click Connect. You may
have to wait a few seconds.

Scan (done locally on your device that runs `Cellulo Robot Hub GUI`, which must have Bluetooth functionality), add
robots and connect to them as usual. Close the `Cellulo Robot Hub GUI` when all desired connections are established.

Instantiate the `CelluloRobotHubClient` with the same `serverAddress` and `port` in your app to be able to use these
connected robots. See [cellulo-robot-pool-hub-demo](../samples/cellulo-robot-pool-hub-demo/) to see how.

**Important note:** If running manually and not as a service, it is advised to run `cellulorobothubd` with root
privileges for much faster connection to many robots over multiple adapters. It can also be run with user privileges
but the connections will possibly be established slower.
