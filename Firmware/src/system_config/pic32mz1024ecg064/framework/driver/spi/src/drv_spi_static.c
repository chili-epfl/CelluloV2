/*******************************************************************************
  SPI Driver Functions for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_spi_static.c

  Summary:
    SPI driver impementation for the static single instance driver.

  Description:
    The SPI device driver provides a simple interface to manage the SPI
    modules on Microchip microcontrollers. This file contains implemenation
    for the SPI driver.
    
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

// *****************************************************************************
// *****************************************************************************
// Header Includes
// *****************************************************************************
// *****************************************************************************
#include "system_definitions.h"
#include "framework/driver/spi/drv_spi_static.h"


// *****************************************************************************
// *****************************************************************************
// Section: Instance 0 static driver functions
// *****************************************************************************
// *****************************************************************************

void DRV_SPI0_Initialize(void)
{
    PLIB_SPI_Disable(SPI_ID_1);
    PLIB_SPI_MasterEnable(SPI_ID_1);
    PLIB_SPI_SlaveSelectDisable(SPI_ID_1);
    PLIB_SPI_StopInIdleDisable(SPI_ID_1);
    PLIB_SPI_ClockPolaritySelect(SPI_ID_1, SPI_CLOCK_POLARITY_IDLE_LOW);
    PLIB_SPI_OutputDataPhaseSelect(SPI_ID_1, SPI_OUTPUT_DATA_PHASE_ON_ACTIVE_TO_IDLE_CLOCK);
    PLIB_SPI_InputSamplePhaseSelect(SPI_ID_1, SPI_INPUT_SAMPLING_PHASE_IN_MIDDLE);
    PLIB_SPI_CommunicationWidthSelect(SPI_ID_1, SPI_COMMUNICATION_WIDTH_32BITS);
    PLIB_SPI_FramedCommunicationDisable( SPI_ID_1  );
    PLIB_SPI_AudioProtocolDisable(SPI_ID_1);
    PLIB_SPI_FIFOEnable( SPI_ID_1  );
    PLIB_SPI_BaudRateSet(SPI_ID_1, SYS_CLK_PeripheralFrequencyGet(CLK_BUS_PERIPHERAL_2), 1000000);

    SYS_INT_VectorPrioritySet(INT_VECTOR_SPI1_TX, INT_PRIORITY_LEVEL2);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_SPI1_TX, INT_SUBPRIORITY_LEVEL0);
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_SPI_1_TRANSMIT);
    //SYS_INT_SourceEnable(INT_SOURCE_SPI_1_TRANSMIT);
    
    PLIB_SPI_Enable(SPI_ID_1);
}

void DRV_SPI0_DeInitialize(void)
{
    /* Disable SPI */
	PLIB_SPI_Disable(SPI_ID_1);
}

bool DRV_SPI0_ReceiverBufferIsFull(void)
{
    return (PLIB_SPI_ReceiverBufferIsFull(SPI_ID_1));
}

bool DRV_SPI0_TransmitterBufferIsFull(void)
{
    return (PLIB_SPI_TransmitBufferIsFull(SPI_ID_1));
}
int32_t DRV_SPI0_BufferAddWriteRead(const void * txBuffer, void * rxBuffer, uint32_t size)
{
    bool continueLoop;
    int32_t txcounter = 0;
    int32_t rxcounter = 0;
    uint8_t unitsTxed = 0;
    const uint8_t maxUnits = 4;
    do {
        continueLoop = false;
        unitsTxed = 0;
        if (PLIB_SPI_TransmitBufferIsEmpty(SPI_ID_1))
        {
            while (!PLIB_SPI_TransmitBufferIsFull(SPI_ID_1) && (txcounter < (size>>2)) && unitsTxed != maxUnits)
            {
                PLIB_SPI_BufferWrite32bit(SPI_ID_1, ((uint32_t*)txBuffer)[txcounter]);
                txcounter++;
                continueLoop = true;
                unitsTxed++;
            }
        }
    
        while (txcounter != rxcounter)
        {
            while (PLIB_SPI_ReceiverFIFOIsEmpty(SPI_ID_1));
            ((uint32_t*)rxBuffer)[rxcounter] = PLIB_SPI_BufferRead32bit(SPI_ID_1);
            rxcounter++;
            continueLoop = true;
        }
        if (txcounter > rxcounter)
        {
            continueLoop = true;
        }
    }while(continueLoop);
    return txcounter<<2;
}

/*******************************************************************************
 End of File
*/
