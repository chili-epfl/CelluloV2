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
 * @file bluetooth.c
 * @brief Source for bluetooth functionality
 * @author Ayberk Özgür
 * @date 2015-05-02
 */

#include "bluetooth.h"

#include <xc.h>
#include <sys/attribs.h>
#include <string.h>

#include "system_definitions.h"
#include "cam.h"
#include "touch.h"
#include "led.h"
#include "motor.h"
#include "motion.h"
#include "haptics.h"
#include "bezier.h"

volatile char btRxQueue[BT_RX_QUEUE_SIZE];
volatile char btRxCmd[BT_RX_QUEUE_SIZE];
unsigned volatile int btRxQueueWriteIndex;
unsigned volatile int btRxQueueReadIndex;
unsigned volatile int btRxCmdIndex;

char cTemp;

const int btCmdLen[] = COMMAND_PACKET_PAYLOAD_LEN_SHARED;
const char* btCmdStrings[] = COMMAND_PACKET_STR_SHARED;
const char* btEventStrings[] = EVENT_PACKET_STR_SHARED;

CmdPacketType currentBtCmd = CmdPacketTypeNumElements;

typedef enum {
    RECEIVE_STATUS_NOT_RECEIVING = 0,
    RECEIVE_STATUS_WAITING_FOR_TYPE,
    RECEIVE_STATUS_PAYLOAD_RECEIVING,
    RECEIVE_STATUS_END_OF_PACKET,
    RECEIVE_STATUS_NUM_RECEIVE_STATUS
} RECEIVE_STATUS;
RECEIVE_STATUS receiveStatus = RECEIVE_STATUS_NOT_RECEIVING;
int receiveBytesRemaining = -1;
unsigned int currentUnloadIndex = 0;

void bluetoothForceBaud9600(BOOL enable){
    if(enable)
        PLIB_PORTS_PinSet(PORTS_ID_0, BT_BAUD_9600_PORT_CHANNEL, BT_BAUD_9600_PORT_BIT_POS);
    else
        PLIB_PORTS_PinClear(PORTS_ID_0, BT_BAUD_9600_PORT_CHANNEL, BT_BAUD_9600_PORT_BIT_POS);
}

void bluetoothReset(){
    bluetoothForceBaud9600(false);
    int k;
    PLIB_PORTS_PinClear(PORTS_ID_0, BT_RESET_BAR_PORT_CHANNEL, BT_RESET_BAR_PORT_BIT_POS);
    for(k=0; k<1000000; k++) ;
    PLIB_PORTS_PinSet(PORTS_ID_0, BT_RESET_BAR_PORT_CHANNEL, BT_RESET_BAR_PORT_BIT_POS);
    btRxQueueWriteIndex = 0;
}

void suspendBluetooth(){

    //Hold reset when sleeping for ultra low power mode
    PLIB_PORTS_PinClear(PORTS_ID_0, BT_RESET_BAR_PORT_CHANNEL, BT_RESET_BAR_PORT_BIT_POS);
    BT_RX_DISABLE();
}

void resumeBluetooth(){
    btRxQueueWriteIndex = 0;

    //Release reset to continue operation
    PLIB_PORTS_PinSet(PORTS_ID_0, BT_RESET_BAR_PORT_CHANNEL, BT_RESET_BAR_PORT_BIT_POS);
    BT_RX_ENABLE();
}

void APP_BT_Initialize(){
    btRxQueueWriteIndex = 0;
    btRxQueueReadIndex = 0;
    btRxCmdIndex = 0;
    BT_RX_ENABLE();
    if(bootState != BOOT_STATE_SOFTWARE_RESET && bootState != BOOT_STATE_WATCHDOG_TIMEOUT)
        bluetoothReset();
}

inline void waitForeverForTransmitterAvailable(){

    //Wait until transmitter buffer is available
    while(PLIB_USART_TransmitterBufferIsFull(USART_ID_4)) ;

    //Wait until Bluetooth module is available
    while(PLIB_PORTS_PinGet(PORTS_ID_0, BT_RTS_BAR_PORT_CHANNEL, BT_RTS_BAR_PORT_BIT_POS)) ;
}

void bluetoothSendWaitForever(const char* c, unsigned int n){
    int k;
    for(k=0; k<n; k++){
        waitForeverForTransmitterAvailable();
        PLIB_USART_TransmitterByteSend(USART_ID_4, c[k]);
    }
}

void bluetoothSendCharWaitForever(char c){
    waitForeverForTransmitterAvailable();
    PLIB_USART_TransmitterByteSend(USART_ID_4, c);
}

inline void sendPacketStart(EventPacketType event){
    bluetoothSendCharWaitForever(PACKET_START_CHAR_SHARED);
    bluetoothSendCharWaitForever(*(btEventStrings[event]));
}

/*
 * Number loaders (directly send in busy wait), convert to big endian when sending
 */

inline void sendUInt32(unsigned int num){
    char* p = (char*)&num;
    bluetoothSendCharWaitForever(p[3]);
    bluetoothSendCharWaitForever(p[2]);
    bluetoothSendCharWaitForever(p[1]);
    bluetoothSendCharWaitForever(p[0]);
}

inline void sendUInt24(unsigned int num){
    char* p = (char*)&num;
    bluetoothSendCharWaitForever(p[2]);
    bluetoothSendCharWaitForever(p[1]);
    bluetoothSendCharWaitForever(p[0]);
}

inline void sendUInt16(unsigned short num){
    char* p = (char*)&num;
    bluetoothSendCharWaitForever(p[1]);
    bluetoothSendCharWaitForever(p[0]);
}

inline void sendUInt8(unsigned char num){
    char* p = (char*)&num;
    bluetoothSendCharWaitForever(*p);
}

inline void sendInt32(int num){
    char* p = (char*)&num;
    bluetoothSendCharWaitForever(p[3]);
    bluetoothSendCharWaitForever(p[2]);
    bluetoothSendCharWaitForever(p[1]);
    bluetoothSendCharWaitForever(p[0]);
}

inline void sendInt24(int num){
    char* p = (char*)&num;
    bluetoothSendCharWaitForever(p[2]);
    bluetoothSendCharWaitForever(p[1]);
    bluetoothSendCharWaitForever(p[0]);
}

inline void sendInt16(short num){
    char* p = (char*)&num;
    bluetoothSendCharWaitForever(p[1]);
    bluetoothSendCharWaitForever(p[0]);
}

inline void sendInt8(char num){
    char* p = (char*)&num;
    bluetoothSendCharWaitForever(*p);
}

inline void sendFloat(float num){
    char* p = (char*)&num;
    bluetoothSendCharWaitForever(p[3]);
    bluetoothSendCharWaitForever(p[2]);
    bluetoothSendCharWaitForever(p[1]);
    bluetoothSendCharWaitForever(p[0]);
}

/*
 * Bluetooth events
 */

void bluetoothEventBootComplete(){
    sendPacketStart(EventPacketTypeBootComplete);
}

void bluetoothEventShuttingDown(){
    sendPacketStart(EventPacketTypeShuttingDown);
}

void bluetoothEventLowBattery(){
    sendPacketStart(EventPacketTypeLowBattery);
}

void bluetoothEventBatteryStateChanged(unsigned int state){
    sendPacketStart(EventPacketTypeBatteryStateChanged);
    sendUInt8(state);
}

void bluetoothEventTouchBegin(unsigned int key){
    reportActivity();

    sendPacketStart(EventPacketTypeTouchBegan);
    sendUInt8(key);
}

void bluetoothEventTouchLongPressed(unsigned int key){
    reportActivity();

    sendPacketStart(EventPacketTypeTouchLongPressed);
    sendUInt8(key);
}

void bluetoothEventTouchReleased(unsigned int key){
    reportActivity();

    sendPacketStart(EventPacketTypeTouchReleased);
    sendUInt8(key);
}

void bluetoothEventGestureChanged(Gesture gesture){
    sendPacketStart(EventPacketTypeGestureChanged);
    sendUInt8((unsigned char)gesture);
}

void bluetoothEventPoseChanged(int x, int y, int theta){
    sendPacketStart(EventPacketTypePoseChanged);
    sendUInt32(x);
    sendUInt32(y);
    sendUInt16(theta);
}

void bluetoothEventACtrlParameters(){
    sendPacketStart(EventPacketTypeACtrlParameters);
    /* i;
    for(i=0; i<18; i++){
        sendInt32((int)(params[i]*1000.));
    }*/
    int i;
    //initACtrl(&actrl);    
    sendFloat(actrl.mrac_x.k_p);
    sendFloat(actrl.mrac_x.k_r);
    for(i=0; i<4; i++){
        sendFloat(actrl.mrac_x.th[i]);
    }

    sendFloat(actrl.mrac_y.k_p);
    sendFloat(actrl.mrac_y.k_r);
    for(i=0; i<4; i++){
        sendFloat(actrl.mrac_y.th[i]);
    }

    sendFloat(actrl.mrac_th.k_p);
    sendFloat(actrl.mrac_th.k_r);
    for(i=0; i<4; i++){
        sendFloat(actrl.mrac_th.th[i]);
    }
}

void bluetoothEventPoseChangedTimestamped(int x, int y, int theta){
    sendPacketStart(EventPacketTypePoseChangedTimestamped);
    sendUInt32(x);
    sendUInt32(y);
    sendUInt16(theta);
    sendUInt32(systemMillis);
}

void bluetoothEventKidnapChanged(bool kidnapped){
    sendPacketStart(EventPacketTypeKidnapChanged);
    sendUInt8(kidnapped ? 1 : 0);
}

void bluetoothEventTrackingGoalReached(){
    sendPacketStart(EventPacketTypeTrackingGoalReached);
}

void bluetoothEventAcknowledged(){
    sendPacketStart(EventPacketTypeAcknowledged);
}

void bluetoothEventCameraImageLine(unsigned short line){
    sendPacketStart(EventPacketTypeFrameLine);
    sendUInt16(line);
    int i;
    for(i=0; i<IMG_WIDTH_SHARED; i++)
        sendUInt8(pixels[line*IMG_WIDTH_SHARED + i]);
}

void bluetoothEventDebug(unsigned int t, unsigned int u){
    sendPacketStart(EventPacketTypeDebug);
    sendUInt32(t);
    sendUInt32(u);
}

/*
 * Number unloaders, convert to little endian after receiving
 */

inline unsigned int unloadUInt32(){
    unsigned char* p = (unsigned char*)btRxCmd;
    unsigned int num = (unsigned int)(p[currentUnloadIndex] << 24) | (p[currentUnloadIndex + 1] << 16) | (p[currentUnloadIndex + 2] << 8) | (p[currentUnloadIndex + 3] << 0);
    currentUnloadIndex += 4;
    return num;
}

inline unsigned int unloadUInt24(){
    unsigned char* p = (unsigned char*)btRxCmd;
    unsigned int num = (unsigned int)((p[currentUnloadIndex] << 16) | (p[currentUnloadIndex + 1] << 8) | (p[currentUnloadIndex + 2] << 0));
    currentUnloadIndex += 3;
    return num;
}

inline unsigned short unloadUInt16(){
    unsigned char* p = (unsigned char*)btRxCmd;
    unsigned short num = (unsigned short)((p[currentUnloadIndex] << 8) | (p[currentUnloadIndex + 1] << 0));
    currentUnloadIndex += 2;
    return num;
}

inline unsigned char unloadUInt8(){
    unsigned char* p = (unsigned char*)btRxCmd;
    unsigned char num = p[currentUnloadIndex];
    currentUnloadIndex += 1;
    return num;
}

inline int unloadInt32(){
    unsigned char* p = (unsigned char*)btRxCmd;
    unsigned int num = (unsigned int)((p[currentUnloadIndex] << 24) | (p[currentUnloadIndex + 1] << 16) | (p[currentUnloadIndex + 2] << 8) | (p[currentUnloadIndex + 3] << 0));
    currentUnloadIndex += 4;
    return *((int*)&num);
}

inline int unloadInt24(){
    unsigned char* p = (unsigned char*)btRxCmd;
    unsigned int num = (unsigned int)((p[currentUnloadIndex] << 16) | (p[currentUnloadIndex + 1] << 8) | (p[currentUnloadIndex + 2] << 0));
    if(num >= 0x800000)
        num += 0xFF000000;
    currentUnloadIndex += 3;
    return *((int*)&num);
}

inline short unloadInt16(){
    unsigned char* p = (unsigned char*)btRxCmd;
    unsigned short num = (unsigned short)((p[currentUnloadIndex] << 8) | (p[currentUnloadIndex + 1] << 0));
    currentUnloadIndex += 2;
    return *((short*)&num);
}

inline char unloadInt8(){
    char num = btRxCmd[currentUnloadIndex];
    currentUnloadIndex += 1;
    return num;
}

inline float unloadFloat(){
    unsigned char* p = (unsigned char*)btRxCmd;
    unsigned int num = (unsigned int)((p[currentUnloadIndex] << 24) | (p[currentUnloadIndex + 1] << 16) | (p[currentUnloadIndex + 2] << 8) | (p[currentUnloadIndex + 3] << 0));
    currentUnloadIndex += 4;
    return *((float*)&num);
}

/*
 * Bluetooth packet receiver
 */

void processCommand(){
    switch(currentBtCmd){
        case CmdPacketTypePing:
            bluetoothEventAcknowledged();
            break;

        case CmdPacketTypeSetBcastPeriod:
            bcastPeriod = unloadUInt16();
            break;

        case CmdPacketTypeTimestampEnable:
            timestamping = (bool)unloadUInt8();
            break;

        case CmdPacketTypeFrameRequest:
            frameRequest = true;
            break;

        case CmdPacketTypeSetExposureTime:
            setExposureTime(unloadUInt32());
            break;

        case CmdPacketTypeBatteryStateRequest:
            bluetoothEventBatteryStateChanged(batteryState);
            break;

        case CmdPacketTypeSetLEDResponseMode:
            setLEDResponseMode((LEDResponseMode)unloadUInt8());
            break;

        case CmdPacketTypeSetLocomotionInteractivityMode:
            setLocomotionInteractivityMode((LocomotionInteractivityMode)unloadUInt8());
            break;

        case CmdPacketTypeGestureEnable:
            gestureEnabled = (bool)unloadUInt8();
            break;

        case CmdPacketTypeCasualBackdriveAssistEnable:
            casualBackdriveAssistEnable((bool)unloadUInt8());
            break;

        case CmdPacketTypeHapticBackdriveAssist:
            hapticBackdriveAssist(
                unloadInt16()/100.0f,
                unloadInt16()/100.0f,
                unloadInt16()/100.0f
                );
            break;

        case CmdPacketTypeSetVisualEffect:
            setVisualEffect((VisualEffect)unloadUInt8(),
                            unloadUInt8(),      //Red
                            unloadUInt8(),      //Green
                            unloadUInt8(),      //Blue
                            unloadUInt8());     //Value
            break;

        case CmdPacketTypeSetMotorOutput:
            setMotorOutput(unloadUInt8(), unloadInt16());
            break;

        case CmdPacketTypeSetAllMotorOutputs:
            setMotorOutput(1, unloadInt16());
            setMotorOutput(2, unloadInt16());
            setMotorOutput(3, unloadInt16());
            break;

        case CmdPacketTypeSetGoalVelocity:
            setGoalVelocity(
                unloadInt16()/(float)GOAL_VEL_FACTOR_SHARED,
                unloadInt16()/(float)GOAL_VEL_FACTOR_SHARED,
                unloadInt16()/(float)GOAL_VEL_FACTOR_SHARED
                );
            break;

        case CmdPacketTypeSetGoalPose:
            setGoalPose(
                unloadUInt32(),
                unloadUInt32(),
                unloadUInt16(),
                unloadUInt16()/(float)GOAL_VEL_FACTOR_SHARED,
                unloadUInt16()/(float)GOAL_VEL_FACTOR_SHARED
                );
            break;

        case CmdPacketTypeSetGoalPosition:
            setGoalPosition(
                unloadUInt32(),
                unloadUInt32(),
                unloadUInt16()/(float)GOAL_VEL_FACTOR_SHARED
                );
            break;

        case CmdPacketTypeSetGoalXCoordinate:
            setGoalXCoordinate(
                unloadUInt32(),
                unloadUInt16()/(float)GOAL_VEL_FACTOR_SHARED
                );
            break;

        case CmdPacketTypeSetGoalYCoordinate:
            setGoalYCoordinate(
                unloadUInt32(),
                unloadUInt16()/(float)GOAL_VEL_FACTOR_SHARED
                );
            break;

        case CmdPacketTypeSetGoalOrientation:
            setGoalOrientation(
                unloadUInt16(),
                unloadUInt16()/(float)GOAL_VEL_FACTOR_SHARED
                );
            break;

        case CmdPacketTypeSetGoalXThetaCoordinate:
            setGoalXThetaCoordinate(
                unloadUInt32(),
                unloadUInt16(),
                unloadUInt16()/(float)GOAL_VEL_FACTOR_SHARED,
                unloadUInt16()/(float)GOAL_VEL_FACTOR_SHARED
                );
            break;

        case CmdPacketTypeSetGoalYThetaCoordinate:
            setGoalYThetaCoordinate(
                unloadUInt32(),
                unloadUInt16(),
                unloadUInt16()/(float)GOAL_VEL_FACTOR_SHARED,
                unloadUInt16()/(float)GOAL_VEL_FACTOR_SHARED
                );
            break;

        case CmdPacketTypeClearTracking:
            clearTracking();
            break;

        case CmdPacketTypeSimpleVibrate:
            simpleVibrate(
                unloadUInt16()/(float)GOAL_VEL_FACTOR_SHARED,
                unloadUInt16()/(float)GOAL_VEL_FACTOR_SHARED,
                unloadUInt16()/(float)GOAL_VEL_FACTOR_SHARED,
                unloadUInt16(),
                unloadUInt16()
                );
            break;

        case CmdPacketTypeVibrateOnMotion:
            vibrateOnMotion(
                unloadUInt16()/100.0f,
                unloadUInt16()
                );
            break;

        case CmdPacketTypeClearHapticFeedback:
            clearHapticFeedback();
            break;

        case CmdPacketTypePolyBezierInit:
            polyBezierInit(
                unloadFloat(),
                unloadFloat()
                );
            break;

        case CmdPacketTypePolyBezierAppend:
            polyBezierAppend(
                unloadFloat(),
                unloadFloat(),
                unloadFloat(),
                unloadFloat(),
                unloadFloat(),
                unloadFloat()
                );
            break;

        case CmdPacketTypeSetGoalPolyBezier:
            setGoalPolyBezier(
                unloadUInt16()/(float)GOAL_VEL_FACTOR_SHARED,
                unloadInt16()/(float)GOAL_VEL_FACTOR_SHARED
                );
            break;

        case CmdPacketTypeSetGoalPolyBezierAligned:
            setGoalPolyBezierAligned(
                unloadUInt16()/(float)GOAL_VEL_FACTOR_SHARED,
                unloadUInt16(),
                unloadUInt16()/(float)GOAL_VEL_FACTOR_SHARED
                );
            break;

        case CmdPacketTypeReset:
            BT_RX_DISABLE();
            softwareReset();
            break;

        case CmdPacketTypeShutdown:
            requestSleep(); 
            break;
        
        case CmdPacketTypeACtrlParameters:
            paramRequest = true; //bluetoothEventACtrlParameters();
            break;
            
        default:
            break;
    }
}

inline void clearReceive(){
    currentBtCmd = CmdPacketTypeNumElements;

    receiveStatus = RECEIVE_STATUS_NOT_RECEIVING;
    receiveBytesRemaining = -1;
    btRxCmdIndex = 0;
    currentUnloadIndex = 0;
}

inline CmdPacketType getCommand(){
    int c;
    for(c=0; c<CmdPacketTypeNumElements; c++)
        if(btCmdStrings[c][0] == btRxQueue[btRxQueueReadIndex])
            return (CmdPacketType)c;
    return CmdPacketTypeNumElements;
}

void __attribute__((optimize("O0"))) APP_BT_Tasks() {
    while(btRxQueueReadIndex != btRxQueueWriteIndex){
        switch(receiveStatus){
            case RECEIVE_STATUS_NOT_RECEIVING:
                if(btRxQueue[btRxQueueReadIndex] == PACKET_START_CHAR_SHARED)
                    receiveStatus = RECEIVE_STATUS_WAITING_FOR_TYPE;
                break;

            case RECEIVE_STATUS_WAITING_FOR_TYPE:

                //Determine command type
                currentBtCmd = getCommand();

                //Valid command type
                if(currentBtCmd != CmdPacketTypeNumElements){
                    receiveBytesRemaining = btCmdLen[(int) currentBtCmd];
                    if(receiveBytesRemaining <= 0){
                        receiveStatus = RECEIVE_STATUS_END_OF_PACKET;
                        processCommand();
                    }
                    else
                        receiveStatus = RECEIVE_STATUS_PAYLOAD_RECEIVING;
                }

                //Invalid command type, reset
                else
                    clearReceive();
                break;

            case RECEIVE_STATUS_PAYLOAD_RECEIVING:
                btRxCmd[btRxCmdIndex] = btRxQueue[btRxQueueReadIndex];
                btRxCmdIndex = (btRxCmdIndex + 1) % BT_RX_QUEUE_SIZE;
                receiveBytesRemaining--;
                if(receiveBytesRemaining <= 0){
                    receiveStatus = RECEIVE_STATUS_END_OF_PACKET;
                    processCommand();
                }
                break;

            case RECEIVE_STATUS_END_OF_PACKET:
                if(btRxQueue[btRxQueueReadIndex] == PACKET_START_CHAR_SHARED){
                    clearReceive();
                    receiveStatus = RECEIVE_STATUS_WAITING_FOR_TYPE;
                }
                break;

            default:
                break;
        }

        btRxQueueReadIndex = (btRxQueueReadIndex + 1) % BT_RX_QUEUE_SIZE;
    }
}

void __ISR(_UART3_RX_VECTOR, ipl3AUTO) _BT_RX_Handler(void){
    //BT_RX_DISABLE();
    while(U3STAbits.URXDA){
        btRxQueue[btRxQueueWriteIndex] = U3RXREG;
        btRxQueueWriteIndex++;
        if(btRxQueueWriteIndex >= BT_RX_QUEUE_SIZE)
            btRxQueueWriteIndex = 0;
    }
    reportActivity();
    //BT_RX_ENABLE();
    IFS4CLR = 0b01000000000000000000000000000000;
}

void __ISR(_UART3_FAULT_VECTOR, ipl2AUTO) _BT_FAULT_Handler(void){

    //Clear everything
    U3STAbits.PERR = 0;
    U3STAbits.FERR = 0;
    U3STAbits.OERR = 0;
    clearReceive();

    IFS4CLR = 0b00100000000000000000000000000000;
}
