/*
 * LCDlib.h
 *
 *  Created on: Nov 5, 2020
 *      Author: Samuel
 */

#ifndef LCDLIB_H_
#define LCDLIB_H_


#include <F28x_Project.h>
#include "OneToOneI2CDriver.h"


#define LCDDELAY    500 // Delay inbetween LCD write (in us)

void InitLCD();
void LCD_string(char* string);
void LCD_command(char* command);

void LCD_Clear();
void LCD_Line1();
void LCD_Line2();
void LCD_CursorL1(OFFSET);
void LCD_CursorL2(OFFSET);

#endif /* LCDLIB_H_ */
