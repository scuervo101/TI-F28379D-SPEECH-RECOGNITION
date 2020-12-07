/*
 * arch.c
 *
 *  Created on: Dec 6, 2020
 *      Author: Samuel
 */

#include "arch_defs.h"

float frames_array(uint16_t r_w, uint32_t n, uint32_t i, float* data){
    if (n > 76)
    {
        if (r_w)
            SRAM_long_write(frame_size * n + i + framehigh_addr, data);
        else
            return SRAM_long_read(frame_size * n + i + framehigh_addr);
    }
    else
    {
        if (r_w)
            SRAM_long_write(frame_size * n + i + framelow_addr, data);
        else
            return SRAM_long_read(frame_size * n + i + framelow_addr);
    }
    return 0;
}
