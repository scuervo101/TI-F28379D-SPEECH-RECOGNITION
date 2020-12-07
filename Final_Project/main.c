/****************************************************************************************
 *
 *      main.c
 *          Final Project, Speech recognition using MFCC feature extraction and GMM
 *          classification
 *
 ****************************************************************************************
 */

#include <stdint.h>
#include <F28x_Project.h>
#include <initFunctions.h>
#include "AIC23.h"
#include "GPIOlib.h"
#include "spiSRAM.h"
#include "mel.h"
#include "knn.h"
#include "arch_defs.h"
#include "train_data.h"
#include "LCDlib.h"

/***************** Options *****************/

/***************** Defines ***************/
#define CodecSPI_CLK_PULS {EALLOW; GpioDataRegs.GPASET.bit.GPIO18 = 1; GpioDataRegs.GPACLEAR.bit.GPIO18 = 1;}
#define CodecSPI_CS_LOW {EALLOW; GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;}
#define CodecSPI_CS_HIGH {EALLOW; GpioDataRegs.GPASET.bit.GPIO19 = 1;}

/***************** Variables *****************/

uint16_t recording = 0;
uint16_t idle = 0;
uint16_t sram_index = 0;


/***************** Interrupt Service Routines *****************/
interrupt void McBSPB_isr(void);

/***************** User Functions *****************/

/***************** Main Routine *****************/
int main(void){

    EALLOW;
    InitSysCtrl();      // Set SYSCLK to 200 MHz, disables watchdog timer, turns on peripheral clocks
    InitSRAM();
    EALLOW;
    InitGPIO();

    DINT;               // Disable CPU interrupts on startup

    InitPieCtrl();      // Initialize PIE -> disable PIE and clear all PIE registers
    IER = 0x0000;       // Clear CPU interrupt register
    IFR = 0x0000;       // Clear CPU interrupt flag register
    InitPieVectTable(); // Initialize PIE vector table to known state

    EALLOW;
    InitSPIA();
    InitMcBSPb(&McBSPB_isr);
    EALLOW;

    Init_MIC_AIC23();
    InitLCD();

    EnableInterrupts();

    EALLOW;

    recording = 0;
    DELAY_US(1000000);
    idle = 1;
    sram_index = 0;
    float frame_energies[n_frames];
    while(1)
    {
        if(idle == 0)
        {
            DINT;
            Led0 = 1;
            Led1 = 1;
            Led2 = 1;
            Led3 = 1;
            Led4 = 1;
            Led5 = 1;
            Led6 = 1;
            Led7 = 1;

            mel_spectrogram(frame_energies);
            uint16_t pred = 3;

            pred = knn(frame_energies, train, labels);
            if (pred){
                LCD_string("Yes");
            }
            else{
                LCD_string("No");
            }
        }
    }
}

/***************** User Functions *****************/

/************************ Interrupts ************************/

interrupt void McBSPB_isr(void){

    int16_t Ldata = McbspbRegs.DRR2.all;
    int16_t Rdata = McbspbRegs.DRR1.all;

    int16_t MData = (int16_t)(Ldata + Rdata)/2.0f;

    if(idle)
    {
        float FData = (float) MData;
        if(recording)
        {
            SRAM_long_write(signal_addr + sram_index, &FData);
            sram_index++;
        }
        else if (MData > 5000)
        {
            recording = 1;
            SRAM_long_write(signal_addr + sram_index, &FData);
            sram_index++;
        }

        if (sram_index >= signal_size)
        {
            recording = 0;
            idle = 0;
            sram_index = 0;
        }
    }

    McbspbRegs.DXR1.all = 0;
    McbspbRegs.DXR2.all = 0;

    PieCtrlRegs.PIEACK.bit.ACK6 = 1;
}
