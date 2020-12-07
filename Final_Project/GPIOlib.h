/*
 * GPIOlib.h
 *
 *  Created on: Oct 21, 2020
 *      Author: Samuel
 */

#ifndef GPIOLIB_H_
#define GPIOLIB_H_

#include <F28x_Project.h>

void InitGPIO();

// LED Defines
#define     Led7    GpioDataRegs.GPADAT.bit.GPIO11
#define     Led6    GpioDataRegs.GPADAT.bit.GPIO10
#define     Led5    GpioDataRegs.GPADAT.bit.GPIO9
#define     Led4    GpioDataRegs.GPADAT.bit.GPIO8
#define     Led3    GpioDataRegs.GPADAT.bit.GPIO7
#define     Led2    GpioDataRegs.GPADAT.bit.GPIO6
#define     Led1    GpioDataRegs.GPADAT.bit.GPIO5
#define     Led0    GpioDataRegs.GPADAT.bit.GPIO4


#endif /* GPIOLIB_H_ */
