/*******************************************************************************
  USART Driver Functions for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usart_static.c

  Summary:
    USART driver impementation for the static single instance driver.

  Description:
    The USART device driver provides a simple interface to manage the USART
    modules on Microchip microcontrollers. This file contains implemenation
    for the USART driver.
    
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
//DOM-IGNORE-END

#include "system_config.h"
#include "peripheral/usart/plib_usart.h"
#include "peripheral/int/plib_int.h"
#include "system_definitions.h"
 

// *****************************************************************************
// *****************************************************************************
// Section: Instance 0 static driver functions
// *****************************************************************************
// *****************************************************************************

void DRV_USART0_Initialize(void)
{
    /* Initialize USART */
    PLIB_USART_BaudRateSet(USART_ID_4, SYS_CLK_PeripheralFrequencyGet(CLK_BUS_PERIPHERAL_2), 115200); //This has no BRGH support
    //U4BRG = 26; //921600 goal, 925926 actual
    //U4MODEbits.BRGH = 1;
    PLIB_USART_HandshakeModeSelect(USART_ID_4, USART_HANDSHAKE_MODE_SIMPLEX);
    PLIB_USART_OperationModeSelect(USART_ID_4, USART_ENABLE_TX_RX_USED);
    PLIB_USART_LineControlModeSelect(USART_ID_4, USART_8N1);
    PLIB_USART_TransmitterEnable(USART_ID_4);
    PLIB_USART_TransmitterInterruptModeSelect(USART_ID_4, USART_TRANSMIT_FIFO_IDLE);

    PLIB_USART_Enable(USART_ID_4);
}




bool DRV_USART0_ReceiverBufferIsEmpty(void)
{
   return (!PLIB_USART_ReceiverDataIsAvailable(USART_ID_4));
}

uint8_t DRV_USART0_ReadByte(void)
{
   if(PLIB_USART_ReceiverOverrunHasOccurred(USART_ID_4))
   {
      PLIB_USART_ReceiverOverrunErrorClear(USART_ID_4);
   }

   return (PLIB_USART_ReceiverByteReceive(USART_ID_4));
}

void DRV_USART0_WriteByte(const uint8_t byte)
{
   while(PLIB_USART_TransmitterBufferIsFull(USART_ID_4))
   {
   }

   PLIB_USART_TransmitterByteSend(USART_ID_4, byte);
}

// *****************************************************************************
// *****************************************************************************
// Section: Instance 1 static driver functions
// *****************************************************************************
// *****************************************************************************

void DRV_USART1_Initialize(void)
{
    /* Initialize USART */
    PLIB_USART_BaudRateSet(USART_ID_3, SYS_CLK_PeripheralFrequencyGet(CLK_BUS_PERIPHERAL_2), 115200); //This has no BRGH support
    //U3BRG = 26; //921600 goal, 925926 actual
    //U3MODEbits.BRGH = 1;
    PLIB_USART_HandshakeModeSelect(USART_ID_3, USART_HANDSHAKE_MODE_SIMPLEX);
    PLIB_USART_OperationModeSelect(USART_ID_3, USART_ENABLE_TX_RX_USED);
    PLIB_USART_LineControlModeSelect(USART_ID_3, USART_8N1);
    PLIB_USART_ReceiverEnable(USART_ID_3);
    PLIB_USART_ReceiverInterruptModeSelect(USART_ID_3, USART_RECEIVE_FIFO_ONE_CHAR);

    /* Initialize RX interrupt */
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_USART_3_RECEIVE);
    PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_USART_3_RECEIVE);
    PLIB_INT_VectorPrioritySet(INT_ID_0, INT_VECTOR_UART3_RX, INT_PRIORITY_LEVEL3);
    PLIB_INT_VectorSubPrioritySet(INT_ID_0, INT_VECTOR_UART3_RX, INT_SUBPRIORITY_LEVEL0);

    /* Initialize RX interrupt */
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_USART_3_ERROR);
    PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_USART_3_ERROR);
    PLIB_INT_VectorPrioritySet(INT_ID_0, INT_VECTOR_UART3_FAULT, INT_PRIORITY_LEVEL3);
    PLIB_INT_VectorSubPrioritySet(INT_ID_0, INT_VECTOR_UART3_FAULT, INT_SUBPRIORITY_LEVEL1);

    PLIB_USART_Enable(USART_ID_3);
}




bool DRV_USART1_ReceiverBufferIsEmpty(void)
{
   return (!PLIB_USART_ReceiverDataIsAvailable(USART_ID_3));
}

uint8_t DRV_USART1_ReadByte(void)
{
   if(PLIB_USART_ReceiverOverrunHasOccurred(USART_ID_3))
   {
      PLIB_USART_ReceiverOverrunErrorClear(USART_ID_3);
   }

   return (PLIB_USART_ReceiverByteReceive(USART_ID_3));
}

void DRV_USART1_WriteByte(const uint8_t byte)
{
   while(PLIB_USART_TransmitterBufferIsFull(USART_ID_3))
   {
   }

   PLIB_USART_TransmitterByteSend(USART_ID_3, byte);
}


/*******************************************************************************
 End of File
*/
