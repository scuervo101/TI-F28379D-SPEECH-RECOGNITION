/*
 * LCDlib.c
 *
 *  Created on: Nov 5, 2020
 *      Author: Samuel
 */


#ifndef LCDLIB_C_
#define LCDLIB_C_

#include <LCDlib.h>

const uint16_t initCmds[] = {0x3C, 0x38, 0x3C, 0x38, 0x3C, 0x38, 0x2C, 0x28, 0x2C, 0x28, 0x8C, 0x88,
                        0x0C, 0x08, 0xFC, 0xF8, 0x0C, 0x08, 0x1C, 0x18};

const uint16_t LCDaddr = 0x27;

void InitLCD(void)
{
    I2C_O2O_Master_Init(LCDaddr,200.0f,100.0f);

    DELAY_US(LCDDELAY);
    I2C_O2O_SendBytes(initCmds, 20);

    DELAY_US(LCDDELAY);
}

void LCD_string(char* string)
{
    char hinibble = 0;
    char lonibble = 0;
    while(*string != 0){
        hinibble = *string;
        lonibble = *string;

        lonibble <<= 4;

        hinibble &= 0xF0;
        hinibble |= 0x09;
        I2C_O2O_SendBytes(&hinibble, 1);
        DELAY_US(LCDDELAY);
        hinibble &= 0xF0;
        hinibble |= 0x0D;
        I2C_O2O_SendBytes(&hinibble, 1);
        DELAY_US(LCDDELAY);

        lonibble &= 0xF0;
        lonibble |= 0x09;
        I2C_O2O_SendBytes(&lonibble, 1);
        DELAY_US(LCDDELAY);
        lonibble &= 0xF0;
        lonibble |= 0x0D;
        I2C_O2O_SendBytes(&lonibble, 1);
        DELAY_US(LCDDELAY);

        string++;
    }
}

void LCD_command(char* command)
{
    char hinibble = 0;
    char lonibble = 0;

    hinibble = *command;
    lonibble = *command;

    lonibble <<= 4;

    hinibble &= 0xF0;
    hinibble |= 0x08;
    I2C_O2O_SendBytes(&hinibble, 1);
    DELAY_US(LCDDELAY);
    hinibble &= 0xF0;
    hinibble |= 0x0C;
    I2C_O2O_SendBytes(&hinibble, 1);
    DELAY_US(LCDDELAY);

    lonibble &= 0xF0;
    lonibble |= 0x08;
    I2C_O2O_SendBytes(&lonibble, 1);
    DELAY_US(LCDDELAY);
    lonibble &= 0xF0;
    lonibble |= 0x0C;
    I2C_O2O_SendBytes(&lonibble, 1);
    DELAY_US(LCDDELAY);
}

void LCD_Clear(){
    char cmd = 0x01;
    LCD_command(&cmd);
}

void LCD_Line1(){
    char cmd = 0x80;
    LCD_command(&cmd);
}

void LCD_Line2(){
    char cmd = 0x80 | 0x40;
    LCD_command(&cmd);
}

void LCD_CursorL1(uint16_t OFFSET){
    char cmd = 0x80 | OFFSET;
    LCD_command(&cmd);
}

void LCD_CursorL2(uint16_t OFFSET){
    char cmd = 0x80 | 0x40 | OFFSET;
    LCD_command(&cmd);
}


#endif /* LCDLIB_C_ */

