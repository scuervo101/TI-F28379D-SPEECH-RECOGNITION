/*
 * spiSRAM.c
 *
 *  Created on: Oct 2, 2020
 *      Author: Samuel
 */

#include <F2837xD_Device.h>
#include "spiSRAM.h"

static void InitSPIGPIO();
static uint16_t SRAM_read();
static void SRAM_write();
static void spib_init();
static uint16_t spi_transmit(uint16_t data);

/*
 *      spi_init:
 *          Initializes SPIB as a master
 */


void InitSRAM(){
    spib_init();

    // Send byte mode command to sram2
    GpioDataRegs.GPCDAT.bit.GPIO67 = 0;
    spi_transmit(0x01);
    spi_transmit(0);
    GpioDataRegs.GPCDAT.bit.GPIO67 = 1;
    // Send byte mode command to sram1
    GpioDataRegs.GPCDAT.bit.GPIO66 = 0;
    spi_transmit(0x01);
    spi_transmit(0);
    GpioDataRegs.GPCDAT.bit.GPIO66 = 1;
}

static void spib_init(){
    EALLOW;
    InitSPIGPIO();

    // Clear SPI
    SpibRegs.SPICCR.bit.SPISWRESET = 0;

    // Set clk to sysclk
    ClkCfgRegs.LOSPCP.bit.LSPCLKDIV = 0;

    // Configure SPI
    SpibRegs.SPICCR.bit.HS_MODE = 1;
    SpibRegs.SPICTL.bit.MASTER_SLAVE = 1;
    SpibRegs.SPICCR.bit.SPICHAR = 7;

    SpibRegs.SPICCR.bit.CLKPOLARITY = 0;
    SpibRegs.SPICTL.bit.CLK_PHASE = 1;

    SpibRegs.SPICTL.bit.TALK = 1;
    SpibRegs.SPIBRR.bit.SPI_BIT_RATE = 0x5;

    // Clear Status Flags
    SpibRegs.SPISTS.bit.OVERRUN_FLAG = 1;
    SpibRegs.SPISTS.bit.INT_FLAG = 0;

    // Free run
    SpibRegs.SPIPRI.bit.FREE = 1;

    // Set SPI
    SpibRegs.SPICCR.bit.SPISWRESET = 1;
}

static void InitSPIGPIO(){
    EALLOW;

    // Set the two chip selects to high
    GpioDataRegs.GPCDAT.bit.GPIO66 = 1;
    GpioDataRegs.GPCDAT.bit.GPIO67 = 1;

    // Set the directions
    GpioCtrlRegs.GPBDIR.bit.GPIO63 = 1;
    GpioCtrlRegs.GPCDIR.bit.GPIO64 = 0;
    GpioCtrlRegs.GPCDIR.bit.GPIO65 = 1;
    GpioCtrlRegs.GPCDIR.bit.GPIO66 = 1;
    GpioCtrlRegs.GPCDIR.bit.GPIO67 = 1;

    // Set the GPIO function
    GpioCtrlRegs.GPBGMUX2.bit.GPIO63 = 3;
    GpioCtrlRegs.GPCGMUX1.bit.GPIO64 = 3;
    GpioCtrlRegs.GPCGMUX1.bit.GPIO65 = 3;
    GpioCtrlRegs.GPCGMUX1.bit.GPIO66 = 0;
    GpioCtrlRegs.GPCGMUX1.bit.GPIO67 = 0;

    GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 3;
    GpioCtrlRegs.GPCMUX1.bit.GPIO64 = 3;
    GpioCtrlRegs.GPCMUX1.bit.GPIO65 = 3;
    GpioCtrlRegs.GPCMUX1.bit.GPIO66 = 0;
    GpioCtrlRegs.GPCMUX1.bit.GPIO67 = 0;
}

static uint16_t spi_transmit(uint16_t data){
    // Char is left justified
    SpibRegs.SPITXBUF = data << 8;
    while(SpibRegs.SPISTS.bit.INT_FLAG == 0);
    // Return Chars
    return SpibRegs.SPIRXBUF;
}


static void SRAM_write(uint32_t addr, uint16_t data)
{
    // Send the write function
    spi_transmit(0x02);

    spi_transmit((addr >> 16) & 0xFF);
    spi_transmit((addr >> 8) & 0xFF);
    spi_transmit(addr & 0xFF);

    spi_transmit(data);
}

static uint16_t SRAM_read(uint32_t addr)
{
    // Send the read function
    spi_transmit(0x03);

    spi_transmit((addr >> 16) & 0xFF);
    spi_transmit((addr >> 8) & 0xFF);
    spi_transmit(addr & 0xFF);

    spi_transmit(0x00);
    return spi_transmit(0x00);
}

void SRAM_word_write(uint32_t address, uint16_t data)
{
    address <<= 1;
    if(address&0x40000){
        GpioDataRegs.GPCDAT.bit.GPIO67 = 0;
    }
    else{
        GpioDataRegs.GPCDAT.bit.GPIO66 = 0;
    }
    // Write byte
    SRAM_write(address, data&0xFF);
    if(address&0x40000){
        GpioDataRegs.GPCDAT.bit.GPIO67 = 1;
    }
    else{
        GpioDataRegs.GPCDAT.bit.GPIO66 = 1;
    }
    // Next Byte
    if(address&0x40000){
        GpioDataRegs.GPCDAT.bit.GPIO67 = 0;
    }
    else{
        GpioDataRegs.GPCDAT.bit.GPIO66 = 0;
    }
    // Write byte
    SRAM_write(address+1, (data>>8)&0xFF);
    if(address&0x40000){
        GpioDataRegs.GPCDAT.bit.GPIO67 = 1;
    }
    else{
        GpioDataRegs.GPCDAT.bit.GPIO66 = 1;
    }
}

uint16_t SRAM_word_read(uint32_t address)
{
    uint16_t data = 0;
    address <<= 1;

    if(address&0x40000){
        GpioDataRegs.GPCDAT.bit.GPIO67 = 0;
    }
    else{
        GpioDataRegs.GPCDAT.bit.GPIO66 = 0;
    }
    // Read byte
    data = SRAM_read(address);

    if(address&0x40000){
        GpioDataRegs.GPCDAT.bit.GPIO67 = 1;
    }
    else{
        GpioDataRegs.GPCDAT.bit.GPIO66 = 1;
    }

    // Next Byte
    if(address&0x40000){
        GpioDataRegs.GPCDAT.bit.GPIO67 = 0;
    }
    else{
        GpioDataRegs.GPCDAT.bit.GPIO66 = 0;
    }
    // Read byte
    data = data | SRAM_read(address+1)<<8;
    if(address&0x40000){
        GpioDataRegs.GPCDAT.bit.GPIO67 = 1;
    }
    else{
        GpioDataRegs.GPCDAT.bit.GPIO66 = 1;
    }

    return data;
}


void SRAM_long_write(uint32_t address, uint32_t* data)
{
    address <<= 1;
    uint32_t send_data = *data;
    SRAM_word_write(address, (uint16_t)(send_data & 0xFFFF));
    SRAM_word_write(address+1, (uint16_t)((send_data >> 16) & 0xFFFF));
}

float SRAM_long_read(uint32_t address)
{
    address <<= 1;

    uint16_t low = SRAM_word_read(address);
    uint16_t high = SRAM_word_read(address+1);
    uint32_t word = ((uint32_t)high << 16);
    word |= low;

    float* data =  &word;
    return *data;
}
