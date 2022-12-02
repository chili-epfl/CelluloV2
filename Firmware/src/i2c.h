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
 * @file i2c.h
 * @brief Header for I2C functionality
 * @author Ayberk Özgür
 * @date 2015-04-26
 */

#ifndef I2C_H
#define I2C_H

#include <GenericTypeDefs.h>
#include "system_config/pic32mz1024ecg064/system_definitions.h"

/**
 * @brief Reads multiple bytes from an I2C slave
 *
 * Calculates read address from the write address, assuming the read address
 * is 1 more than the write address.
 *
 * Initiates a software reset if the transaction was not successful.
 *
 * @param writeAddr I2C write address of the slave
 * @param registerAddr Address of the slave register to read
 * @param n Number of bytes to read
 * @param buffer Place to read bytes to
 *
 * @return Whether the read was successful
 */
bool readBytes(unsigned char writeAddr, unsigned char registerAddr, int n, unsigned char* buffer);

/**
 * @brief Writes multiple bytes to an I2C slave
 *
 * Initiates a software reset if the transaction was not succesful.
 *
 * @param writeAddr I2C write address of the slave
 * @param registerAddr Address of the slave register to write to
 * @param n Number of bytes to write
 * @param buffer Bytes to write
 *
 * @param Whether the write was succesful
 */
bool writeBytes(unsigned char writeAddr, unsigned char registerAddr, int n, unsigned char* buffer);

#endif //I2C_H
