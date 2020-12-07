/*
 * rfft_defs.h
 *
 *  Created on: Dec 7, 2020
 *      Author: Samuel
 */

#ifndef RFFT_DEFS_H_
#define RFFT_DEFS_H_

#define RFFT_STAGES     10
#define RFFT_SIZE       (1 << RFFT_STAGES)

#pragma DATA_SECTION(RFFTin1Buff,"RFFTdata1")
float RFFTin1Buff[RFFT_SIZE];

#pragma DATA_SECTION(RFFTmagBuff,"RFFTdata2")
float RFFTmagBuff[RFFT_SIZE/2+1];

#pragma DATA_SECTION(RFFToutBuff,"RFFTdata3")
float RFFToutBuff[RFFT_SIZE];

#pragma DATA_SECTION(RFFTF32Coef,"RFFTdata4")
float RFFTF32Coef[RFFT_SIZE];


#endif /* RFFT_DEFS_H_ */
