/*
 * OneToOneI2CDriver.h
 *
 *  Created on: Sep 24, 2016
 *      Author: Raz Aloni
 */

#ifndef ONETOONEI2CDRIVER_H_
#define ONETOONEI2CDRIVER_H_

/*
 * <summary>
 * 	Initializes the I2C to run in Master Mode for a One-To-One connection
 * </summary>
 * <param="slaveAddress">Address of the slave device to write to</param>
 * <param="sysClkMhz">System Clock Frequency in Mhz</param>
 * <param="I2CClkKHz">Desired I2C Clock Frequency in KHz</param>
 */
void I2C_O2O_Master_Init(uint16_t slaveAddress, float sysClkMhz, float I2CClkKHz);

/*
 * <summary>
 * 	Sends bytes via I2C
 * </summary>
 * <param="values">Pointer to array of bytes to send</param>
 * <param-"length">Length of array</param>
 */
void I2C_O2O_SendBytes(uint16_t * const values, uint16_t length);


#endif /* ONETOONEI2CDRIVER_H_ */
