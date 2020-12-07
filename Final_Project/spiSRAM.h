/*
 * spiSRAM.h
 *
 *  Created on: Oct 2, 2020
 *      Author: Samuel
 */

#ifndef SPISRAM_H_
#define SPISRAM_H_


/*
 *      InitSRAM:
 *          Initializes SRAM
 */
void InitSRAM();


/*
 *      SRAM_long_write
 *          Writes a float to SRAM address
 */
void SRAM_long_write(uint32_t address, uint32_t* data);


/*
 *      SRAM_long_read
 *          Reads a float from SRAM address
 */
float SRAM_long_read(uint32_t address);

/*
 *      SRAM_word_write
 *          Writes a word to SRAM address
 */
void SRAM_word_write(uint32_t address, uint16_t data);

/*
 *      SRAM_word_read
 *          Reads a word to SRAM address
 */
uint16_t SRAM_word_read(uint32_t address);

#endif /* SPISRAM_H_ */
