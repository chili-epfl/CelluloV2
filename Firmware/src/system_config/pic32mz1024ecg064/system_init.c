/*******************************************************************************
  System Initialization File

  File Name:
    system_init.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, configuration bits, and allocates
    any necessary global system resources, such as the systemObjects structure
    that contains the object handles to all the MPLAB Harmony module objects in
    the system.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

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
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "system_config.h"
#include "system_definitions.h"





#include<peripheral/dma/plib_dma.h>
#include"../../qml-plugin/include/CelluloBluetoothSharedDefs.h"


// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************

/*** DEVCFG0 ***/

#pragma config DEBUG =      OFF
#pragma config JTAGEN =     OFF
#pragma config ICESEL =     ICS_PGx1
#pragma config TRCEN =      ON
#pragma config BOOTISA =    MIPS32
#pragma config FECCCON =    OFF_UNLOCKED
#pragma config FSLEEP =     VREGS
#pragma config DBGPER =     PG_ALL
#pragma config EJTAGBEN =   NORMAL
#pragma config CP =         OFF

/*** DEVCFG1 ***/

#pragma config FNOSC =      SPLL
#pragma config DMTINTV =    WIN_127_128
#pragma config FSOSCEN =    OFF
#pragma config IESO =       ON
#pragma config POSCMOD =    OFF
#pragma config OSCIOFNC =   OFF
#pragma config FCKSM =      CSECMD
#pragma config WDTPS =      PS2048
#pragma config WDTSPGM =    STOP
#pragma config FWDTEN =     OFF
#pragma config WINDIS =     NORMAL
#pragma config FWDTWINSZ =  WINSZ_25
#pragma config DMTCNT =     DMT31
#pragma config FDMTEN =     OFF

/*** DEVCFG2 ***/

#pragma config FPLLIDIV =   DIV_1
#pragma config FPLLRNG =    RANGE_5_10_MHZ
#pragma config FPLLICLK =   PLL_FRC
#pragma config FPLLMULT =   MUL_50
#pragma config FPLLODIV =   DIV_2
#pragma config UPLLFSEL =   FREQ_24MHZ
#pragma config UPLLEN =     OFF

/*** DEVCFG3 ***/

#pragma config USERID =     0xffff
#pragma config FMIIEN =     ON
#pragma config FETHIO =     ON
#pragma config PGL1WAY =    ON
#pragma config PMDL1WAY =   ON
#pragma config IOL1WAY =    ON
#pragma config FUSBIDIO =   OFF

/*** BF1SEQ0 ***/

#pragma config TSEQ =       0xffff
#pragma config CSEQ =       0xffff


// *****************************************************************************
// *****************************************************************************
// Section: Library/Stack Initialization Data
// *****************************************************************************
// *****************************************************************************/



// *****************************************************************************
// *****************************************************************************
// Section: Driver Initialization Data
// *****************************************************************************
// *****************************************************************************

//<editor-fold defaultstate="collapsed" desc="DRV_USART Configuration">

// </editor-fold>
//<editor-fold defaultstate="collapsed" desc="DRV_SPI Configuration">

 /*** SPI Driver Initialization Data ***/
// </editor-fold>
//<editor-fold defaultstate="collapsed" desc="DRV_I2C Configuration">
// </editor-fold>

// *****************************************************************************
// *****************************************************************************
// Section: System Data
// *****************************************************************************
// *****************************************************************************

/* Structure to hold the object handles for the modules in the system. */
SYSTEM_OBJECTS sysObj;

// *****************************************************************************
// *****************************************************************************
// Section: Module Initialization Data
// *****************************************************************************
// *****************************************************************************


//<editor-fold defaultstate="collapsed" desc="SYS_DEVCON Configuration">

/*** System Device Control Initialization Data ***/

const SYS_DEVCON_INIT sysDevconInit =
{
    .moduleInit = {0},
};
// </editor-fold>

// *****************************************************************************
// *****************************************************************************
// Section: Static Initialization Functions
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: System Initialization
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void SYS_Initialize ( SYS_INIT_DATA *data )

  Summary:
    Initializes the board, services, drivers, application and other modules.

  Remarks:
    See prototype in system/common/sys_module.h.
 */

void SYS_Initialize ( void* data )
{
    /* Core Processor Initialization */
    SYS_CLK_Initialize( NULL );
    sysObj.sysDevcon = SYS_DEVCON_Initialize(SYS_DEVCON_INDEX_0, (SYS_MODULE_INIT*)&sysDevconInit);
    SYS_DEVCON_PerformanceConfig(SYS_CLK_SystemFrequencyGet());
    SYS_PORTS_Initialize();















    /* Disable the DMA module */
    PLIB_DMA_Disable(DMA_ID_0);

    /* Set the priority levels for the DMA Channels */
    PLIB_DMA_ChannelXPrioritySelect(DMA_ID_0, DMA_CHANNEL_0, 0x00);
    PLIB_DMA_ChannelXPrioritySelect(DMA_ID_0, DMA_CHANNEL_1, 0x00);
    PLIB_DMA_ChannelXPrioritySelect(DMA_ID_0, DMA_CHANNEL_2, 0x00);
    PLIB_DMA_ChannelXPrioritySelect(DMA_ID_0, DMA_CHANNEL_3, 0x00);
    PLIB_DMA_ChannelXPrioritySelect(DMA_ID_0, DMA_CHANNEL_4, 0x00);
    PLIB_DMA_ChannelXPrioritySelect(DMA_ID_0, DMA_CHANNEL_5, 0x00);
    PLIB_DMA_ChannelXPrioritySelect(DMA_ID_0, DMA_CHANNEL_6, 0x00);

    /* Enable the DMA module */
    PLIB_DMA_Enable(DMA_ID_0);


    /* DMA transfer to start */
    PLIB_DMA_ChannelXTriggerEnable(DMA_ID_0, DMA_CHANNEL_0, DMA_CHANNEL_TRIGGER_TRANSFER_START);
    PLIB_DMA_ChannelXStartIRQSet(DMA_ID_0, DMA_CHANNEL_0, DMA_TRIGGER_EXTERNAL_2);

    /* Set the source and destinaton addresses (addresses are converted from virtual to physical) */
    PLIB_DMA_ChannelXSourceStartAddressSet(DMA_ID_0, DMA_CHANNEL_0, (uint32_t)&PORTE);
    //PLIB_DMA_ChannelXDestinationStartAddressSet(DMA_ID_0, DMA_CHANNEL_0, 0);

    /* Set the source and destination sizes */
    PLIB_DMA_ChannelXSourceSizeSet(DMA_ID_0, DMA_CHANNEL_0, 1);
    PLIB_DMA_ChannelXDestinationSizeSet(DMA_ID_0, DMA_CHANNEL_0, IMG_WIDTH_SHARED);

    /* Set the number of bytes per transfer */
    PLIB_DMA_ChannelXCellSizeSet(DMA_ID_0, DMA_CHANNEL_0, 1);






    //DMA finished interrupt
    SYS_INT_VectorPrioritySet(INT_VECTOR_DMA0, INT_PRIORITY_LEVEL6);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_DMA0, INT_SUBPRIORITY_LEVEL3);
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_DMA_0);
    SYS_INT_SourceEnable(INT_SOURCE_DMA_0);
    DCH0INTbits.CHBCIE = 1;








    /* Initialize Drivers */
    DRV_USART0_Initialize();
    //DRV_USART1_Initialize();
    DRV_SPI0_Initialize();
    DRV_I2C0_Initialize();

    /* Initialize System Services */
    SYS_INT_Initialize();

    /*Setup the INT_SOURCE_EXTERNAL_3 and Enable it*/ //Line start
    SYS_INT_VectorPrioritySet(INT_VECTOR_INT3, INT_PRIORITY_LEVEL7);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_INT3, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_ExternalInterruptTriggerSet(INT_EXTERNAL_INT_SOURCE3,INT_EDGE_TRIGGER_RISING);
    //SYS_INT_SourceEnable(INT_SOURCE_EXTERNAL_3);

    /*Setup the INT_SOURCE_EXTERNAL_4 and Enable it*/ //Frame start
    SYS_INT_VectorPrioritySet(INT_VECTOR_INT4, INT_PRIORITY_LEVEL6);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_INT4, INT_SUBPRIORITY_LEVEL1);
    SYS_INT_ExternalInterruptTriggerSet(INT_EXTERNAL_INT_SOURCE4,INT_EDGE_TRIGGER_RISING);
    //SYS_INT_SourceEnable(INT_SOURCE_EXTERNAL_4);

    /*Setup the INT_SOURCE_EXTERNAL_2 but do not Enable it, we only need it to trigger DMA*/ //PIXCLK interrupt
    SYS_INT_VectorPrioritySet(INT_VECTOR_INT2, INT_PRIORITY_LEVEL6);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_INT2, INT_SUBPRIORITY_LEVEL2);
    SYS_INT_ExternalInterruptTriggerSet(INT_EXTERNAL_INT_SOURCE2,INT_EDGE_TRIGGER_RISING);





    /* Initialize Middleware */
    /* Enable Global Interrupts */
    SYS_INT_Enable();


}

/*******************************************************************************
 End of File
*/
