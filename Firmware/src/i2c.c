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
 * @file i2c.c
 * @brief Source for I2C functionality
 * @author Ayberk Özgür
 * @date 2015-04-26
 */

#include "i2c.h"

#include <xc.h>
#include <sys/attribs.h>
#include "system_definitions.h"

//Executes X until it returns true. If timeout T is reached, executes return false.
#define WAIT_UNTIL_TRUE(X,T) \
    i=0; \
    while(!X){ \
        i++; \
        if(i>T) \
            return false; \
    }

bool _I2CWriteByte(unsigned char byte){
    if(!DRV_I2C0_ByteWrite(byte))
        return false;
    if(!DRV_I2C0_WaitForByteWriteToComplete())
        return false;
    if(!DRV_I2C0_WriteByteAcknowledged())
        return false;
    return true;
}

bool I2CReadBytes(unsigned char writeAddr, unsigned char registerAddr, int n, unsigned char* buffer){
    unsigned int i;

    //Assert start sequence
    WAIT_UNTIL_TRUE(DRV_I2C0_MasterBusIdle(), 1000000);
    if(!DRV_I2C0_MasterStart()) return false;
    if(!DRV_I2C0_WaitForStartComplete()) return false;

    //Send slave write address
    WAIT_UNTIL_TRUE(DRV_I2C0_MasterBusIdle(), 100000);
    if(!_I2CWriteByte(writeAddr)) return false;

    //Send register address
    WAIT_UNTIL_TRUE(DRV_I2C0_MasterBusIdle(), 100000);
    if(!_I2CWriteByte(registerAddr)) return false;

    //Assert restart sequence
    WAIT_UNTIL_TRUE(DRV_I2C0_MasterBusIdle(), 1000000);
    if(!DRV_I2C0_MasterRestart()) return false;
    if(!DRV_I2C0_WaitForStartComplete()) return false;

    //Send slave read address
    WAIT_UNTIL_TRUE(DRV_I2C0_MasterBusIdle(), 100000);
    if(!_I2CWriteByte(writeAddr + 1)) return false;

    //Read n - 1 bytes with ACK in the end
    while(n > 1){

        //Set up byte read
        WAIT_UNTIL_TRUE(DRV_I2C0_MasterBusIdle(), 1000000);
        if(!DRV_I2C0_SetUpByteRead()) return false;
        WAIT_UNTIL_TRUE(DRV_I2C0_MasterBusIdle(), 1000000);
        *buffer = DRV_I2C0_ByteRead();
        buffer++;

        //Assert ACK
        DRV_I2C0_MasterACKSend();
        WAIT_UNTIL_TRUE(DRV_I2C0_MasterBusIdle(), 100000);

        n--;
    }

    //Read last byte with NACK in the end
    {

        //Set up byte read
        WAIT_UNTIL_TRUE(DRV_I2C0_MasterBusIdle(), 1000000);
        DRV_I2C0_SetUpByteRead();
        i=0;
        WAIT_UNTIL_TRUE(DRV_I2C0_MasterBusIdle(), 1000000);
        *buffer = DRV_I2C0_ByteRead();

        //Assert NACK
        DRV_I2C0_MasterNACKSend();
        WAIT_UNTIL_TRUE(DRV_I2C0_MasterBusIdle(), 100000);
    }

    //Send stop sequence
    WAIT_UNTIL_TRUE(DRV_I2C0_MasterBusIdle(), 100000);
    if(!DRV_I2C0_MasterStop()) return false;

    //Stop sequence stops and restarts whole I2C module due to silicon bug
    //so no need to wait for stop condition end

    return true;
}

bool I2CWriteBytes(unsigned char writeAddr, unsigned char registerAddr, int n, unsigned char* buffer){
    unsigned int i;

    //Assert start sequence
    WAIT_UNTIL_TRUE(DRV_I2C0_MasterBusIdle(), 1000000);
    if(!DRV_I2C0_MasterStart()) return false;
    if(!DRV_I2C0_WaitForStartComplete()) return false;

    //Send slave write address
    WAIT_UNTIL_TRUE(DRV_I2C0_MasterBusIdle(), 100000);
    if(!_I2CWriteByte(writeAddr)) return false;

    //Send register address
    WAIT_UNTIL_TRUE(DRV_I2C0_MasterBusIdle(), 1000000);
    if(!_I2CWriteByte(registerAddr)) return false;

    //Write n bytes
    while(n > 0){
        WAIT_UNTIL_TRUE(DRV_I2C0_MasterBusIdle(), 1000000);
        if(!_I2CWriteByte(*buffer)) return false;
        buffer++;
        n--;
    }

    //Assert start sequence
    WAIT_UNTIL_TRUE(DRV_I2C0_MasterBusIdle(), 1000000);
    if(!DRV_I2C0_MasterStart()) return false;
    if(!DRV_I2C0_WaitForStartComplete()) return false;

    //Send stop sequence
    WAIT_UNTIL_TRUE(DRV_I2C0_MasterBusIdle(), 100000);
    if(!DRV_I2C0_MasterStop()) return false;

    //Stop sequence stops and restarts whole I2C module due to silicon bug
    //so no need to wait for stop condition end

    return true;
}

void __ISR(_I2C5_MASTER_VECTOR, ipl1AUTO) _IntHandlerDrvI2CMasterInstance0(void){
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_I2C_5_MASTER);
}

void __ISR(_I2C5_BUS_VECTOR, ipl1AUTO) _IntHandlerDrvI2CErrorInstance0(void){
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_I2C_5_BUS);
}
