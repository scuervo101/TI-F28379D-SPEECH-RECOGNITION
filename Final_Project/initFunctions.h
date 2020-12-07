/*
 * init.h
 *
 *  Created on: Oct 20, 2020
 *      Author: Samuel
 */

#ifndef INITFUNCTIONS_H_
#define INITFUNCTIONS_H_

#include <F28x_Project.h>
#include "AIC23.h"

void InitMcBSPb();
void InitSPIA();
void Init_MIC_AIC23();
void InitAIC23();
void InitAIC23_kHz(uint16_t freq);

#endif /* INITFUNCTIONS_H_ */
