/*
 * mel.c
 *
 *  Created on: Dec 6, 2020
 *      Author: Samuel
 */


#include "mel.h"
#include "rfft_defs.h"

/***************** Function Prototype *****************/

void reflect_pad();
void frame_aduio();
void hamming_window(float* window);
void power_dft(float* sum, uint16_t frame);


/***************** Variable *****************/

RFFT_F32_STRUCT rfft;

/***************** User Functions *****************/

/*
 *      mel_spectrogram - Function to find the frame energies from a signal
 *
 *          params:
 *              signal - The signal should be stored in the external SRAM
 *          defines:
 *              size - The size of the signal. The DSP is always sampling a second of data so 44100
 *          return:
 *              frame_energies - A array of size 101 with the framed data's power spectrum
 */
void mel_spectrogram(float* frame_energies){

    Led0 = 0;

    // Apply preemphasis filter
    float temp = 0.0f; // temp write buffer
    for(uint16_t i = 1; i < signal_size; i++)
    {
        float x = SRAM_long_read(signal_addr + i);
        float x_1 = SRAM_long_read(signal_addr + i - 1); // Read x[i-1] before over writing it

        SRAM_long_write(signal_addr + i - 1, &temp); // Over write x[i-1]

        temp = x - (0.95f * x_1);
    }
    SRAM_long_write(signal_addr + signal_size - 1, &temp);

    Led1 = 0;

    // Pad the ends of the signal using mirrored signal
    reflect_pad();

    Led2 = 0;

    // Separate the audio into frames
    frame_aduio();

    Led3 = 0;

    // Apply the hamming window to the frames
    float window[frame_width];
    float data = 0.0f;
    hamming_window(window);
    for (uint16_t n = 0; n < num_frames; n++)
    {
        for (uint16_t i = 0; i < frame_size; i++)
        {
            data = frames_array(0, n, i, 0);
            data *= window[i];
            frames_array(1, n, i, &data);
        }
    }

    Led4 = 0;


    rfft.FFTSize = RFFT_SIZE;
    rfft.FFTStages = RFFT_STAGES;
    rfft.MagBuf = RFFTmagBuff;

    rfft.CosSinBuf = RFFTF32Coef;
    RFFT_f32_sincostable(&rfft);


    float out_buf[RFFT_SIZE];
//    float fft_buf[frame_width];
//    kiss_fft_cpx out_buf[frame_width];
    // Calculate the power spectrum of the frames
    for (uint16_t n = 0; n < num_frames; n++)
    {
        rfft.InBuf = RFFTin1Buff;
        rfft.OutBuf = RFFToutBuff;
        for (uint16_t i = 0; i < num_frames; i++)
        {
            RFFTin1Buff[i] = frames_array(0, n, i, 0);
        }
        RFFT_f32u(&rfft);
        RFFT_f32_mag(&rfft);

        frame_energies[n] = 0;
        for (uint16_t i = 0; i < fft_size; i++)
        {
            frame_energies[n] += RFFTmagBuff[i] * RFFTmagBuff[i];
        }

//        for (uint16_t i = 0; i < num_frames; i++)
//        {
//            fft_buf[i] = frames_array(0, n, i, 0);
//        }
//        kiss_fft_cfg cfg = kiss_fft_alloc(frame_width, 0, 0, 0);
//        kiss_fft(cfg, fft_buf, out_buf);
//
//        frame_energies[n] = 0;
//        for (uint16_t i = 0; i < fft_size; i++)
//        {
//            frame_energies[n] += out_buf[i].r * out_buf[i].r + out_buf[i].i * out_buf[i].i;
//        }

//        power_dft(frame_energies, n);
    }

    Led5 = 0;

}

void reflect_pad(){

    uint16_t y_it = signal_addr - 1;
    uint16_t i = signal_size + 1;
    uint16_t size = 0;
    float temp = 0.0f;
    while(size < pad_size)
    {
        if ((i % signal_size) != 0)
        {
            if (((signal_size - 1 - i) % (signal_size*2)) >  (signal_size - 1))
            {
                temp = SRAM_long_read(i % signal_size);
                SRAM_long_write(y_it, &temp);
                size ++;
                y_it --;
            }
            else
            {
                temp = SRAM_long_read((signal_size - 1 - i) & signal_size);
                SRAM_long_write(y_it, &temp);
                size ++;
                y_it --;
            }
        }
        i ++;
    }

    y_it = highpad_add;
    i = 0;
    size = 0;
    while(size < pad_size)
    {
        if ((i % signal_size) != 0)
        {
            if (((signal_size - 1 - i) % (signal_size*2)) >  (signal_size - 1))
            {
                temp = SRAM_long_read(i % signal_size);
                SRAM_long_write(y_it, &temp);
                size ++;
                y_it ++;
            }
            else
            {
                temp = SRAM_long_read((signal_size - 1 - i) & signal_size);
                SRAM_long_write(y_it, &temp);
                size ++;
                y_it ++;
            }
        }
        i ++;
    }
}

void frame_aduio(){

    float data = 0.0f;
    for (uint16_t n = 0; n < n_frames; n++)
    {
        for (uint16_t i = 0; i < frame_width; i++)
        {
            data = SRAM_long_read(n * stride + i);
            frames_array(1, n, i, &data);
        }
    }
}

void hamming_window(float* window){

    for(uint16_t i = 0; i < window_size; i++)
    {
        window[i] = 0.0f;
        window[i] += 0.54f * cosf(0.0f * ham_step * i);
        window[i] += 0.46f * cosf(1.0f * ham_step * i);
    }
}

void power_dft(float* sum, uint16_t frame){
    float real;
    float imagine;
    float theta;
    float buf;
    float temp;

    sum[frame] = 0;

    // Only need the real results which is just the lower half of frame_width sized FFT
    for (uint16_t i  = 0; i < fft_size; i++)
    {

        real = 0.0f;
        imagine = 0.0f;

        for (uint16_t j = 0; j < frame_width; j++)
        {
            buf = frames_array(0, frame, j, 0);

            theta = 2.0f * M_PI * (i * j) / frame_width;
            real += (float)(buf) * cosf(theta);
            imagine -= (float)(buf) * sinf(theta);
        }

        temp = real*real + imagine*imagine;
        sum[frame] += temp;
    }
}
