/*******************************************************************************
  I2C Driver Functions for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_i2c_static.c
	
  Summary:
    I2C driver impementation for the static single instance driver.

  Description:
    The I2C device driver provides a simple interface to manage the I2C
    modules on Microchip microcontrollers. This file contains implemenation
    for the I2C driver.
    
  Remarks:
    Static interfaces incorporate the driver instance number within the names
    of the routines, eliminating the need for an object ID or object handle.
    
    Static single-open interfaces also eliminate the need for the open handle.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
#include "framework/driver/i2c/drv_i2c_static.h"

// *****************************************************************************
// *****************************************************************************
// Section: Instance 0 static driver functions
// *****************************************************************************
// *****************************************************************************

//Executes X until it returns true. If timeout T is reached, executes return false. Needs unsigned int i declared.
#define WAIT_UNTIL_TRUE(X,T) \
    i=0; \
    while(!X){ \
        i++; \
        if(i>T) \
            return false; \
    }

void DRV_I2C0_Initialize(void)
{
    /* Initialize I2C0 */
    PLIB_I2C_BaudRateSet(I2C_ID_5, SYS_CLK_PeripheralFrequencyGet(CLK_BUS_PERIPHERAL_2), 400000);
    PLIB_I2C_StopInIdleDisable(I2C_ID_5);

    /* High frequency is enabled (**NOTE** PLIB function revereted) */
    PLIB_I2C_HighFrequencyDisable(I2C_ID_5);


    /* Initialize master interrupt */
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_I2C_5_MASTER);
    PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_I2C_5_MASTER);
    PLIB_INT_VectorPrioritySet(INT_ID_0, INT_VECTOR_I2C5_MASTER, INT_PRIORITY_LEVEL1);
    PLIB_INT_VectorSubPrioritySet(INT_ID_0, INT_VECTOR_I2C5_MASTER, INT_SUBPRIORITY_LEVEL0);

    /* Initialize fault interrupt */
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_I2C_5_BUS);
    PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_I2C_5_BUS);
    PLIB_INT_VectorPrioritySet(INT_ID_0, INT_VECTOR_I2C5_BUS, INT_PRIORITY_LEVEL1);
    PLIB_INT_VectorSubPrioritySet(INT_ID_0, INT_VECTOR_I2C5_BUS, INT_SUBPRIORITY_LEVEL0);
    /* Enable I2C0 */
    PLIB_I2C_Enable(I2C_ID_5);
}

void DRV_I2C0_DeInitialize(void)
{
    /* Disable I2C0 */
    PLIB_I2C_Disable(I2C_ID_5);
}

// *****************************************************************************
// Section: Instance 0 Byte Transfer Functions (Master/Slave)
// *****************************************************************************
bool DRV_I2C0_SetUpByteRead(void)
{
    /* Check for recieve overflow */
    if ( PLIB_I2C_ReceiverOverflowHasOccurred(I2C_ID_5))  
    {
        PLIB_I2C_ReceiverOverflowClear(I2C_ID_5); 
        return false;
    }
	
    /* Initiate clock to receive */
    PLIB_I2C_MasterReceiverClock1Byte(I2C_ID_5);
    return true;
}

bool DRV_I2C0_WaitForReadByteAvailable(void)
{
	/* Wait for Recieve Buffer Full */
    if(PLIB_I2C_ReceivedByteIsAvailable(I2C_ID_5))
       return true;
    else
       return false;
}

uint8_t DRV_I2C0_ByteRead(void)
{	
    /* Return recieved value */
    return (PLIB_I2C_ReceivedByteGet(I2C_ID_5));
}

bool DRV_I2C0_ByteWrite(const uint8_t byte)
{
    /* Check for recieve overflow */
    if ( PLIB_I2C_ReceiverOverflowHasOccurred(I2C_ID_5))  
    {
        PLIB_I2C_ReceivedByteGet(I2C_ID_5);
		PLIB_I2C_ReceiverOverflowClear(I2C_ID_5); 
        return false;
    }
    
    /* Check for transmit overflow */
    if (PLIB_I2C_TransmitterOverflowHasOccurred(I2C_ID_5))
    {
		PLIB_I2C_TransmitterOverflowClear(I2C_ID_5);
        return false;
    }
    
    /* Transmit byte */
    PLIB_I2C_TransmitterByteSend(I2C_ID_5, byte);
    
    return true;
}

bool DRV_I2C0_WaitForByteWriteToComplete(void){

    unsigned int i;
    
    /* Check for transmit busy */
    WAIT_UNTIL_TRUE(!PLIB_I2C_TransmitterIsBusy(I2C_ID_5),1000000);

    /* Check to see if transmit has completed */
    WAIT_UNTIL_TRUE(PLIB_I2C_TransmitterByteHasCompleted(I2C_ID_5),1000000);
	
    return true;
}

bool DRV_I2C0_WriteByteAcknowledged(void)
{
    /* Check to see if transmit ACKed = true or NACKed = false */
    if (PLIB_I2C_TransmitterByteWasAcknowledged(I2C_ID_5))
       return true;
    else
       return false;
}

// *****************************************************************************
// Section: Instance 0 I2C Master Mode Functions
// *****************************************************************************
void DRV_I2C0_BaudRateSet(I2C_BAUD_RATE baudRate)
{
    /* Disable I2C0 */
    PLIB_I2C_Disable(I2C_ID_5);
    
    /* Change baud rate */
    PLIB_I2C_BaudRateSet(I2C_ID_5, SYS_CLK_PeripheralFrequencyGet(CLK_BUS_PERIPHERAL_2), baudRate);

    /* High frequency is enabled (**NOTE** PLIB function revereted) */
    PLIB_I2C_HighFrequencyDisable(I2C_ID_5);
 
    /* Enable I2C0 */
    PLIB_I2C_Enable(I2C_ID_5);       
}

bool DRV_I2C0_MasterBusIdle(void)
{
    if (PLIB_I2C_BusIsIdle(I2C_ID_5))
       return true;
    else
       return false;
}

bool DRV_I2C0_MasterStart(void)
{
    /* Check for recieve overflow */
    if ( PLIB_I2C_ReceiverOverflowHasOccurred(I2C_ID_5))  
    {
        PLIB_I2C_ReceiverOverflowClear(I2C_ID_5); 
        return false;
    }
    
    /* Check for transmit overflow */
    if (PLIB_I2C_TransmitterOverflowHasOccurred(I2C_ID_5))
    {
        PLIB_I2C_TransmitterOverflowClear(I2C_ID_5);
        return false;
    }

    /* Check for bus collision errors */
    if (PLIB_I2C_ArbitrationLossHasOccurred(I2C_ID_5))
    {
        return false;
    }
    
    /* Issue start */
    PLIB_I2C_MasterStart(I2C_ID_5);
       
    return true;
}

bool DRV_I2C0_WaitForStartComplete(void)
{
    /* Wait for start/restart sequence to finish  (hardware clear) */
    unsigned int i;
    WAIT_UNTIL_TRUE(PLIB_I2C_StartWasDetected(I2C_ID_5), 100000);
    return true;
}

bool DRV_I2C0_MasterRestart(void)
{
    /* Check for recieve overflow */
    if ( PLIB_I2C_ReceiverOverflowHasOccurred(I2C_ID_5))  
    {
        PLIB_I2C_ReceiverOverflowClear(I2C_ID_5); 
        return false;
    }
    
    /* Check for transmit overflow */
    if (PLIB_I2C_TransmitterOverflowHasOccurred(I2C_ID_5))
    {
        PLIB_I2C_TransmitterOverflowClear(I2C_ID_5);
        return false;
    }
    
    /* Issue restart */
    PLIB_I2C_MasterStartRepeat(I2C_ID_5); 
       
    return true;    
}

bool DRV_I2C0_MasterStop(void)
{
    /* Check for transmit overflow */
    if (PLIB_I2C_TransmitterOverflowHasOccurred(I2C_ID_5))
    {
        PLIB_I2C_TransmitterOverflowClear(I2C_ID_5);
        return false;
    }
    
    /* Issue stop */
    PLIB_I2C_MasterStop(I2C_ID_5);  

#if __PIC32MZ
    unsigned int i;
    WAIT_UNTIL_TRUE(PLIB_I2C_StopWasDetected(I2C_ID_5), 100000);
    PLIB_I2C_Disable(I2C_ID_5);
    PLIB_I2C_Enable(I2C_ID_5);
#endif	
    
    return true;
}

bool DRV_I2C0_WaitForStopComplete(void)
{
    unsigned int i;
    WAIT_UNTIL_TRUE(PLIB_I2C_StopWasDetected(I2C_ID_5), 100000);
    return true;
}

void DRV_I2C0_MasterACKSend(void)
{
    /* Check if receive is ready to ack */
    if ( PLIB_I2C_MasterReceiverReadyToAcknowledge(I2C_ID_5) )
    {
        PLIB_I2C_ReceivedByteAcknowledge (I2C_ID_5, true);
    }    

}

void DRV_I2C0_MasterNACKSend(void)
{
    /* Check if receive is ready to nack */
    if ( PLIB_I2C_MasterReceiverReadyToAcknowledge(I2C_ID_5) )
    {
       PLIB_I2C_ReceivedByteAcknowledge (I2C_ID_5, false);
    }    
}

bool DRV_I2C0_WaitForACKOrNACKComplete(void)
{
    /* Check for ACK/NACK to complete */
    unsigned int i;
    WAIT_UNTIL_TRUE(!PLIB_I2C_ReceiverByteAcknowledgeHasCompleted(I2C_ID_5), 100000);
    return true;
}



/*******************************************************************************
 End of File
*/