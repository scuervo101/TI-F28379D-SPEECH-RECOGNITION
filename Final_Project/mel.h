/*
 * mel.h
 *
 *  Created on: Dec 6, 2020
 *      Author: Samuel
 */

#ifndef MEL_H_
#define MEL_H_

#include <stdint.h>
#include "spiSRAM.h"
#include "arch_defs.h"
#include <math.h>
#include "GPIOlib.h"
#include "fpu_rfft.h"
//#include "kiss_fft.h"

void mel_spectrogram(float* frame_energies);

void dft(int16_t * inbuf, uint16_t steps, uint16_t size);

#endif /* MEL_H_ */
