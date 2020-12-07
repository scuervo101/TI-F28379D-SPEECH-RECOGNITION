/*
 * arch_defs.h
 *
 *  Created on: Dec 6, 2020
 *      Author: Samuel
 */

#ifndef ARCH_DEFS_H_
#define ARCH_DEFS_H_

#include <stdint.h>
#include "spiSRAM.h"

/*
 *  Total SRAM size
 */
#define sram_size           0x20000     // Or 131072
#define sramh_addr          0x1ffff     // Or 131071, Highest address

/*
 *  Signal defs
 */
#define sample_rate     44100
#define signal_len      (signal_size)

/*
 *  Mel Addressing
 */
#define lowpad_addr     0
#define highpad_add     (pad_size + signal_size)
#define signal_addr     (pad_size)

/*
 *  Address Sizes
 */
#define pad_size        551
#define signal_size     (sample_rate)
#define padded_size     (pad_size + signal_size + pad_size)

/*
 *  Mel Function Data
 */
#define stride          441                         // Stride = stride_time * fs where stride time is a fixed size of .01 seconds
#define frame_width     1102                        // Frame_width = window_time * fs where window time is a fixed size of 0.025 seconds
#define fft_size        ((frame_width / 2) + 1)     // fft_size = real values are the lower half
#define n_frames        101                         // n_frames = Number of frames, (len(padded_signal) - frame_width) / stride + 1

/*
 *  Hamming define
 */
#define ham_step        0.00570162f     // The steps between input to create a input range of -pi to pi with 1102 inputs
#define window_size     (frame_width)


/***************** Framing *****************/

/*
 *  Frame Addressing
 *      Because frame and signal have to share the SRAM, we are going to have lower half of frames
 *      in the upper half of the SRAM and then the higher half in the lower half of SRAM
 */
#define num_frames      (n_frames)                  // Number of frames

#define framelow_addr   46000                       // Address above padded_size
#define framehigh_addr  0                           // Address of frames once frame data overwrites signal data

#define frames_low      76                          // 76 frames, Number of frames in the framelow_addr to max range
#define frames_high     (num_frames - frames_low)   // 25 frames, Number of frames in the framehigh_addr to framelow_addr range

#define frame_size      (frame_width)               // 1102 width, Size of each frame

/*
 *  Pseudo 2D array:
 *      frames[n][i] = frames[frame_size * n + i]
 *
 *      where frames[frame_size * n + i] =
 *          if n > 76:
 *              frames[frame_size * n + i + framehigh_addr]
 *          else:
 *              frames[frame_size * n + i + framelow_addr]
 *
 *      r_w = 1:
 *          SRAM_write
 *      r_w = 0:
 *          SRAM_read
 */
float frames_array(uint16_t r_w, uint32_t n, uint32_t i, float* data);

#endif /* ARCH_DEFS_H_ */
