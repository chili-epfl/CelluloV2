/*
 * Copyright (C) 2015 EPFL
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

/**
 * @file CelluloBluetooth.h
 * @brief Header for Bluetooth communication with Cellulo robots
 * @author Ayberk Özgür
 * @date 2015-05-20
 */

#ifndef CELLULOBLUETOOTH_H
#define CELLULOBLUETOOTH_H

#include <QTimer>
#include <QQueue>
#include <QQuickItem>
#include <QBluetoothSocket>
#include <QColor>
#include <QMetaEnum>
#include <QVariantList>

#include "CelluloBluetoothEnums.h"
#include "CelluloBluetoothPacket.h"
#include "relay/CelluloRelayClient.h"
#include "relay/CelluloRelayServer.h"
#include "../zones/CelluloZoneClient.h"
#include "../zones/CelluloZonePolyBezier.h"
#include "../tile/PoseRemapper.h"

namespace Cellulo{

class CelluloRelayClient;
class CelluloRelayServer;

/**
 * @addtogroup comm
 * @{
 */

/**
 * @brief Object that communicates with one Cellulo robot over Bluetooth.
 *
 * Inherits from `CelluloZoneClient` and therefore can be used as one.
 */
class CelluloBluetooth : public CelluloZoneClient {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief Local adapter's MAC address chosen to connect to the robot, empty string if using default adapter; only works on Linux and if the plugin is built with BlueZ */
    Q_PROPERTY(QString localAdapterMacAddr WRITE setLocalAdapterMacAddr READ getLocalAdapterMacAddr NOTIFY localAdapterMacAddrChanged)

    /** @brief Whether to start connecting immediately as soon as **macAddr** is set and to reconnect if connection drops, default true */
    Q_PROPERTY(bool autoConnect WRITE setAutoConnect READ getAutoConnect NOTIFY autoConnectChanged)

    /** @brief Robot MAC address in the form "XX:XX:XX:XX:XX:XX" */
    Q_PROPERTY(QString macAddr WRITE setMacAddr READ getMacAddr NOTIFY macAddrChanged)

    /** @brief Current connection status to the robot, read-only */
    Q_PROPERTY(Cellulo::CelluloBluetoothEnums::ConnectionStatus connectionStatus READ getConnectionStatus NOTIFY connectionStatusChanged)

    /** @brief Robot's current battery state, read-only */
    Q_PROPERTY(Cellulo::CelluloBluetoothEnums::BatteryState batteryState READ getBatteryState NOTIFY batteryStateChanged)

    /** @brief Robot's x coordinate in mm, read-only */
    Q_PROPERTY(float x READ getX NOTIFY poseChanged_inherited)

    /** @brief Robot's y coordinate in mm, read-only */
    Q_PROPERTY(float y READ getY NOTIFY poseChanged_inherited)

    /** @brief Robot's orientation in degrees, read-only */
    Q_PROPERTY(float theta READ getTheta NOTIFY poseChanged_inherited)

    /** @brief Whether the robot is not on encoded paper, read-only */
    Q_PROPERTY(bool kidnapped READ getKidnapped NOTIFY kidnappedChanged)

    /** @brief Current robot gesture (if gesture detection is enabled), read-only */
    Q_PROPERTY(Cellulo::CelluloBluetoothEnums::Gesture gesture READ getGesture NOTIFY gestureChanged)

    /** @brief Whether the robot will send its own timestamp along with its pose, default false */
    Q_PROPERTY(bool timestampingEnabled WRITE setTimestampingEnabled READ getTimestampingEnabled)

    /** @brief Last local timestamp received along with pose (is valid if **timestampingEnabled** is true), read-only */
    Q_PROPERTY(int lastTimestamp READ getLastTimestamp NOTIFY timestampChanged)

    /** @brief Localization framerate calculated from local timestamps received along with pose (is valid if **timestampingEnabled** is true), read-only */
    Q_PROPERTY(float framerate READ getFramerate NOTIFY timestampChanged)

    /** @briefv Pose remapper if desired, null by default, pass null if you want to disable this functionaliy */
    Q_PROPERTY(Cellulo::PoseRemapper* poseRemapper MEMBER poseRemapper)

    /** @cond DO_NOT_DOCUMENT */

    Q_PROPERTY(float cameraImageProgress READ getCameraImageProgress NOTIFY cameraImageProgressChanged)

    /** @endcond */

    friend class CelluloRelayServer;
    friend class CelluloRelayClient;

public:

    /** @cond DO_NOT_DOCUMENT */

    static const int BT_CONNECT_TIMEOUT_MILLIS = 30000;       ///< Will try to reconnect after this much time
    static const int BT_CONNECT_TIMEOUT_MILLIS_PM = 5000;     ///< Variation in reconnect timeout
    static const int BT_KEEP_ALIVE_MILLIS_DEFAULT = 4000;     ///< Bluetooth socket will send a packet at least this frequently
    static const int BT_KEEP_ALIVE_MILLIS_ANDROID_26 = 500;   ///< Bluetooth socket will send a packet at least this frequently on Android >= 26
    static const int BT_BIND_FAIL_RECONNECT_MILLIS = 5000;    ///< After local adapter binding fail, will try to reconnect after this much time
    static const int BT_BIND_FAIL_RECONNECT_MILLIS_PM = 2500; ///< Variation in local adapter binding fail reconnect timeout
    static const int BT_WRONG_ADAPTER_CHECK_MILLIS = 1000;    ///< Period at which to check whether the socket is connected over the wrong local adapter
    static constexpr float FRAMERATE_SMOOTH_FACTOR = 0.99f;   ///< Smoothing factor for framerate, closer to 1.0 means less update

    static QByteArray frameBuffer;                            ///< Container for the received camera frame data

    /**
     * @brief Creates a new Cellulo robot communicator
     *
     * @param parent Parent of this QML item
     */
    explicit CelluloBluetooth(QQuickItem* parent = 0);

    /**
     * @brief Destroys this Cellulo robot communicator
     */
    virtual ~CelluloBluetooth();

    /**
     * @brief Get the chosen local adapter's MAC address
     *
     * @return Chosen local adapter's MAC address, empty string if using default adapter
     */
    QString getLocalAdapterMacAddr() const { return localAdapterMacAddr; }

    /**
     * @brief Gets whether the socket tries to reconnect when it drops, connects when mac address is set
     *
     * @return Whether the socket tries to reconnect when it drops, connects when mac address is set
     */
    bool getAutoConnect(){ return autoConnect; }

    /**
     * @brief Gets the latest camera frame
     *
     * @return The latest camera frame; IMG_WIDTH*IMG_HEIGHT many ints in grayscale, 0 to 255
     */
    QVariantList getFrame() const;

    /**
     * @brief Gets the current MAC address
     *
     * @return The current MAC address
     */
    QString getMacAddr(){ return macAddr; }

    /**
     * @brief Gets current Bluetooth connection status
     *
     * @return Current Bluetooth connection status
     */
    CelluloBluetoothEnums::ConnectionStatus getConnectionStatus(){ return connectionStatus; }

    /**
     * @brief Gets whether timestamping along with pose is currently enabled
     *
     * @return Whether timestamping is enabled
     */
    bool getTimestampingEnabled(){ return timestampingEnabled; }

    /**
     * @brief Gets the latest battery state
     *
     * @return Battery state
     */
    CelluloBluetoothEnums::BatteryState getBatteryState(){ return batteryState; }

    /**
     * @brief Gets the latest x position
     *
     * @return Latest x position in mm
     */
    float getX(){ return x; }

    /**
     * @brief Gets the latest y position
     *
     * @return Latest y position in mm
     */
    float getY(){ return y; }

    /**
     * @brief Gets the latest orientation
     *
     * @return Latest orientation in degrees
     */
    float getTheta(){ return theta; }

    /**
     * @brief Gets the latest available timestamp
     *
     * @return The latest received timestamp in milliseconds
     */
    int getLastTimestamp(){ return lastTimestamp; }

    /**
     * @brief Gets the localization framerate
     *
     * @return Localization framerate in milliseconds
     */
    float getFramerate(){ return framerate; }

    /**
     * @brief Gets the latest kidnapped state
     *
     * @return Whether kidnapped or on encoded paper
     */
    bool getKidnapped(){ return kidnapped; }

    /**
     * @brief Gets the current gesture
     *
     * @return Current gesture
     */
    CelluloBluetoothEnums::Gesture getGesture(){ return gesture; }

    /**
     * @brief Gets the camera image progress
     *
     * @return Between 0.0 and 1.0
     */
    float getCameraImageProgress(){ return cameraImageProgress; }

    /** @endcond */

private slots:

    /**
     * @brief Called when the server sends data
     */
    void socketDataArrived();

    /**
     * @brief Called when (re)connected to server
     */
    void socketConnected();

    /**
     * @brief Called when disconnected from server
     */
    void socketDisconnected();

    /**
     * @brief Disconnects first and then connects again if not connected yet
     */
    void refreshConnection();

    /**
     * @brief If socket is detected to be on the wrong local adapter, rebinds and reconnects
     */
    void checkWrongAdapter();

public slots:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Sets the MAC address of the local adapter to use when connecting to the robot
     *
     * Only works on Linux; must be built with bluez.
     *
     * @param localAdapterMacAddr MAC address of the local adapter, must exist; set to empty string to connect with default adapter
     */
    void setLocalAdapterMacAddr(QString localAdapterMacAddr);

    /**
     * @brief Sets whether the socket will try to reconnect when it drops, connects when mac address is set
     *
     * @param autoReconnect Whether the socket will try to reconnect when it drops, connects when mac address is set
     */
    void setAutoConnect(bool autoConnect);

    /**
     * @brief Sets the MAC address of the server and immediately connects to it
     *
     * @param macAddr Bluetooth MAC address of the server (robot)
     */
    void setMacAddr(QString macAddr);

    /** @endcond */

    /**
     * @brief Creates socket and connects to the current target MAC address
     */
    void connectToServer();

    /**
     * @brief Disconnects from the currently connected device and destroys the socket if possible
     */
    void disconnectFromServer();

    /**
     * @brief Sets the pose broadcast period
     *
     * @param period Desired period in milliseconds, set to 0 for as fast as possible, i.e around 93Hz
     */
    void setPoseBcastPeriod(unsigned int period);

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Enables timestamping along with pose and disables pose idling or vice-versa
     *
     * @param enabled Whether to enable timestamping
     */
    void setTimestampingEnabled(bool enabled);

    /**
     * @brief Sets output of motor 1
     *
     * @param output Value between -0xFFF and 0xFFF
     */
    void setMotor1Output(int output);

    /**
     * @brief Sets output of motor 2
     *
     * @param output Value between -0xFFF and 0xFFF
     */
    void setMotor2Output(int output);

    /**
     * @brief Sets output of motor 3
     *
     * @param output Value between -0xFFF and 0xFFF
     */
    void setMotor3Output(int output);

    /**
     * @brief Sets outputs of all motors
     *
     * @param m1output Value between -0xFFF and 0xFFF
     * @param m2output Value between -0xFFF and 0xFFF
     * @param m3output Value between -0xFFF and 0xFFF
     */
    void setAllMotorOutputs(int m1output, int m2output, int m3output);

    /** @endcond */

    /**
     * @brief Sets robot goal velocity in global world frame
     *
     * @param vx X velocity in mm/s
     * @param vy Y velocity in mm/s
     * @param w Angular velocity in rad/s
     */
    void setGoalVelocity(float vx, float vy, float w);

    /**
     * @brief Sets a pose goal to track
     *
     * @param x X goal in mm
     * @param y Y goal in mm
     * @param theta Theta goal in degrees
     * @param v Maximum linear speed to track pose in mm/s
     * @param w Maximum angular speed to track pose in rad/s
     */
    void setGoalPose(float x, float y, float theta, float v, float w);

    /**
     * @brief Sets a position goal to track
     *
     * @param x X goal in mm
     * @param y Y goal in mm
     * @param v Maximum linear speed to track pose in mm/s
     */
    void setGoalPosition(float x, float y, float v);

    /**
     * @brief Sets an X coordinate to track
     *
     * @param x X goal in mm
     * @param v Maximum linear speed to track pose in mm/s
     */
    void setGoalXCoordinate(float x, float v);

    /**
     * @brief Sets a Y coordinate to track
     *
     * @param y Y goal in mm
     * @param v Maximum linear speed to track pose in mm/s
     */
    void setGoalYCoordinate(float y, float v);

    /**
     * @brief Sets a position goal to track
     *
     * @param theta Theta goal in degrees
     * @param w Maximum angular speed to track pose in rad/s
     */
    void setGoalOrientation(float theta, float w);

    /**
     * @brief Sets an X and Theta goal at the same time
     *
     * @param x X goal in mm
     * @param theta Theta goal in degrees
     * @param v Maximum linear speed to track pose in mm/s
     * @param w Maximum angular speed to track pose in rad/s
     */
    void setGoalXThetaCoordinate(float x, float theta, float v, float w);

    /**
     * @brief Sets a Y and Theta goal at the same time
     *
     * @param y Y goal in mm
     * @param theta Theta goal in degrees
     * @param v Maximum linear speed to track pose in mm/s
     * @param w Maximum angular speed to track pose in rad/s
     */
    void setGoalYThetaCoordinate(float y, float theta, float v, float w);

    /**
     * @brief Clears pose/position/velocity goals
     */
    void clearTracking();

    /**
     * @brief Constantly vibrates the robot
     *
     * @param iX X intensity, scale is the same as linear velocity
     * @param iY Y intensity, scale is the same as linear velocity
     * @param iTheta Theta intensity, scale is the same as angular velocity
     * @param period Period of vibration in milliseconds, maximum is 0xFFFF
     * @param duration Duration of vibration in milliseconds, maximum is 0xFFFF, 0 for vibrate forever
     */
    void simpleVibrate(float iX, float iY, float iTheta, unsigned int period, unsigned int duration);

    /**
     * @brief Enables vibration against motion
     *
     * @param iCoeff Vibration intensity with respect to the drive velocities
     * @param period Period of vibration in milliseconds, maximum is 0xFFFF
     */
    void vibrateOnMotion(float iCoeff, unsigned int period);

    /**
     * @brief Clears all haptic feedbacks
     */
    void clearHapticFeedback();

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Sends a ping, expecting an acknowledge
     */
    void ping();

    /**
     * @brief Requests a camera frame to be sent
     */
    void requestFrame();

    /** @endcond */

    /**
     * @brief Sets the exposure time for super-fast unkidnap detection for uniform and known paper colors known or enables autoexposure
     *
     * 460 pixels for the latest white plastic frame and white paper is a good value.
     *
     * @param pixels Exposure time in pixels, must be larger than 260; 0 is a special value that enables autoexposure
     */
    void setExposureTime(int pixels);

    /**
     * @brief Sends a battery state query
     */
    void queryBatteryState();

    /**
     * @brief Sets the LED response mode, i.e the LED visual response of the robot to touches
     *
     * @param mode LED resposne mode
     */
    void setLEDResponseMode(Cellulo::CelluloBluetoothEnums::LEDResponseMode mode);

    /**
     * @brief Sets the locomotion interactivity mode, i.e the dependance of locomotion to user input
     *
     * @param mode Locomotion interactivity mode
     */
    void setLocomotionInteractivityMode(Cellulo::CelluloBluetoothEnums::LocomotionInteractivityMode mode);

    /**
     * @brief Enables/disables raw touch signal offset querying and processing
     *
     * @param enabled Whether to enable
     */
    void setGestureEnabled(bool enabled);

    /**
     * @brief Enables/disables assist for easy backdriving
     *
     * @param enabled Whether to enable
     */
    void setCasualBackdriveAssistEnabled(bool enabled);

    /**
     * @brief Haptic feedback-oriented backdrive assist
     *
     * Pass 0 to all parameters to disable.
     * Disables casual backdrive assist upon enabling.
     *
     * @param xAssist X assist with respect to x drive velocity, can be negative
     * @param yAssist Y assist with respect to y drive velocity, can be negative
     * @param thetaAssist Theta assist with respect to w drive, can be negative
     */
    void setHapticBackdriveAssist(float xAssist, float yAssist, float thetaAssist);

    /**
     * @brief Sets the visual effect on the robot, changing LED illumination
     *
     * @param effect The effect ordinal
     * @param color Color
     * @param value A value possibly meaningful for the effect (between 0 and 255)
     */
    void setVisualEffect(Cellulo::CelluloBluetoothEnums::VisualEffect effect, QColor color, int value);

    /**
     * @brief [DEPRECATED, USE PolyBezierTracker INSTEAD] Sends the first control point of the composite Bézier curve for motion tracking
     *
     * @param point0 The initial point, i.e the first control point of the first segment
     */
    void polyBezierInit(const QVector2D& point0);

    /**
     * @brief [DEPRECATED, USE PolyBezierTracker INSTEAD] Adds a segment to the composite Bézier curve for motion tracking; polyBezierInit() must be called before
     *
     * The first control point is the last control point of the previously added segment or the initial point.
     *
     * @param point1 The second control point for the curve
     * @param point2 The third control point for the curve
     * @param point3 The fourth control point for the curve
     */
    void polyBezierAppend(const QVector2D& point1, const QVector2D& point2, const QVector2D& point3);

    /**
     * @brief [DEPRECATED, USE PolyBezierTracker INSTEAD] Sends the composite Bézier curve in the zone to the robot
     *
     * @param zone The zone that contains the composite Bézier curve path
     */
    void polyBezierSetFromZone(Cellulo::CelluloZone* zone);

    /**
     * @brief [DEPRECATED, USE PolyBezierTracker INSTEAD] Starts tracking the previously set composite Bézier curve
     *
     * @param v Linear velocity tangent to the curve
     * @param w Angular velocity throughout the traversal
     */
    void setGoalPolyBezier(float v, float w);

    /**
     * @brief [DEPRECATED, USE PolyBezierTracker INSTEAD] Starts tracking the previously set composite Bézier curve while keeping orientation aligned to the curve
     *
     * @param v Linear velocity tangent to the curve
     * @param theta Orientation to track with respect to the curve tangent orientation, in degrees
     * @param w Maximum angular velocity while tracking orientation, in rad/s
     */
    void setGoalPolyBezierAligned(float v, float theta, float w);

    /**
     * @brief Initiates a software reset on the robot
     */
    void reset();

    /**
     * @brief Initiates sleep on the robot
     */
    void shutdown();

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when the desired local adapter changes
     */
    void localAdapterMacAddrChanged();

    /**
     * @brief Auto connect strategy changed
     */
    void autoConnectChanged();

    /**
     * @brief Emitted when the MAC address changes
     */
    void macAddrChanged();

    /**
     * @brief Emitted when Bluetooth connection status changes
     */
    void connectionStatusChanged();

    /** @endcond */

    /**
     * @brief Emitted when the robot is ready after a power up or a reset
     */
    void bootCompleted();

    /**
     * @brief Emitted when the robot is about to sleep (shutdown) due to the user command via touch keys
     */
    void shuttingDown();

    /**
     * @brief Emitted when the robot is about to sleep (shutdown) due to low battery
     */
    void lowBattery();

    /**
     * @brief Emitted when the robot acknowledges the ping
     */
    void acknowledged();

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when the battery state changes
     */
    void batteryStateChanged();

    /** @endcond */

    /**
     * @brief Emitted when a key is touched
     *
     * @param key The key that is touched
     */
    void touchBegan(int key);

    /**
     * @brief Emitted when a key is touched for more than the long touch time
     *
     * @param key The key that is touched
     */
    void longTouch(int key);

    /**
     * @brief Emitted when a key is released
     *
     * @param key The key that is released
     */
    void touchReleased(int key);

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when the gesture on the robot changes
     */
    void gestureChanged();

    /**
     * @brief A new onboard localization timestamp has been received
     */
    void timestampChanged();

    /**
     * @brief Emitted when a new camera image line is received
     */
    void cameraImageProgressChanged();

    /**
     * @brief Emitted when a camera frame from the robot is ready to read
     */
    void frameReady();

    /**
     * @brief Emitted when the kidnap state of the robot changes
     */
    void kidnappedChanged();

    /** @endcond */

    /**
     * @brief DO NOT USE EXPLICITLY, use poseChanged(x,y,theta) instead
     *
     * Emitted when the base class emits poseChanged() since NOTIFY cannot be used with inherited signals
     */
    void poseChanged_inherited();

    /**
     * @brief Emitted when pose/position/orientation goal is reached
     */
    void trackingGoalReached();

private:

    QString localAdapterMacAddr;                              ///< MAC address of the local adapter to use when connecting to the robot, only works on Linux
    bool autoConnect;                                         ///< Whether the socket will try to reconnect every time it drops, will connect when mac address is set
    QTimer wrongAdapterCheckTimer;                            ///< Timer to check whether robot is connected over wrong adapter

    CelluloBluetoothPacket sendPacket;                        ///< Outgoing packet
    CelluloBluetoothPacket recvPacket;                        ///< Incoming packet

    QTimer btKeepAliveTimer;                                  ///< Timeout timer to keep socket alive by sending a ping
    QTimer btConnectTimeoutTimer;                             ///< Timeout timer to reconnect if connection fails
    QBluetoothSocket* socket;                                 ///< Bluetooth socket connected to the server
    QString macAddr;                                          ///< Bluetooth MAC address of the server
    CelluloBluetoothEnums::ConnectionStatus connectionStatus; ///< Bluetooth connection status

    CelluloRelayClient* relayClient;                          ///< Client to route all command packets to
    CelluloRelayServer* relayServer;                          ///< Server to route all event packets to
    PoseRemapper* poseRemapper;                               ///< Pose remapper

    bool timestampingEnabled;                                 ///< Whether timestamping along with pose is enabled and idling disabled
    int lastTimestamp;                                        ///< Latest received onboard timestamp (in milliseconds)
    float framerate;                                          ///< Framerate calculated over time
    float cameraImageProgress;                                ///< Camera image streaming progress

    CelluloBluetoothEnums::BatteryState batteryState;         ///< Current battery state
    float x;                                                  ///< Current x position in mm
    float y;                                                  ///< Current y position in mm
    float theta;                                              ///< Current orientation in degrees
    bool kidnapped;                                           ///< Whether currently kidnapped

    CelluloBluetoothEnums::Gesture gesture;                   ///< Current gesture

    /**
     * @brief Resets properties of the robot to default
     */
    void resetProperties();

    /**
     * @brief Starts the connection timeout timer, sets the timer to (time +- pm) milliseconds
     *
     * @param time Mean time to wait in milliseconds
     * @param pm Plus-minus time to wait in milliseconds
     */
    void startTimeoutTimer(int time, int pm);

    /**
     * @brief Sets the relay client
     *
     * All command packets from now on will be sent through the client instead of a direct Bluetooth connection to a physical robot.
     * Essentially makes this CelluloBluetooth a virtual robot.
     *
     * @param relayClient The relay client
     */
    void setRelayClient(CelluloRelayClient* relayClient);

    /**
     * @brief Sets the relay server
     *
     * All event packets that arrive from the robot from now on will be sent through the server instead of being emitted.
     *
     * @param relayServer The relay server
     */
    void setRelayServer(CelluloRelayServer* relayServer);

    /**
     * @brief Connects or reconnects to the service on the server if not already connected
     */
    void openSocket();

    /**
     * @brief Sends an EventPacketTypeAnnounceConnectionStatus to the relay server
     */
    void announceConnectionStatusToRelayServer();

    /**
     * @brief Sets the motor output
     *
     * @param motor 1, 2 or 3
     * @param output Value between -FFF and FFF
     */
    void setMotorOutput(int motor, int output);

    /**
     * @brief Sends the already built packet over Bluetooth
     */
    void sendCommand();

    /**
     * @brief Sends the command packet that is found in the external packet over Bluetooth
     *
     * @param externalPacket External packet, must be already built
     */
    void sendCommand(CelluloBluetoothPacket const& externalPacket);

    /**
     * @brief Processes the response in the receive buffer if possible
     */
    void processResponse();

    /**
     * @brief Processes the response in the given buffer if possible
     */
    void processResponse(CelluloBluetoothPacket& externalPacket);
};

/** @} */

}

#endif // CELLULOBLUETOOTH_H
