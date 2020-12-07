/*
 * knn.c
 *
 *  Created on: Dec 7, 2020
 *      Author: Samuel
 */


#include "knn.h"

uint16_t knn(float* test_d, float* train_d, uint16_t* train_l){

    float distance[40];
    float innersum = 0.0f;

    // Calculate the distance to each point
    for (uint16_t i = 0; i < train_size; i++)
    {
        innersum = 0.0f;
        for (uint16_t j = 0; j < mel_len; j++)
        {
            innersum += (test_d[j] - train_d[i*mel_len + j]) * (test_d[j] - train_d[i*mel_len + j]);
        }
        distance[i] = sqrtf(innersum);
    }

    Led6 = 0;

    // Sort distance smallest to largest value
    uint16_t already_sorted = 0;
    float temp = 0.0f;
    uint16_t templ = 0;
    for (uint16_t i = 0; i < train_size; i++)
    {
        already_sorted = 1;
        for (uint16_t j = 0; j < (train_size - i - 1); j++)
        {
            if (distance[j] > distance[j+1])
            {
                temp = distance[j];
                distance[j] = distance[j+1];
                distance[j+1] = temp;

                templ = train_l[j];
                train_l[j] = train_l[j+1];
                train_l[j+1] = templ;
            }
        }
        if (already_sorted)
            break;
    }

    Led7 = 0;

    int16_t vote = 0;
    if (train_l[0]){
        vote++;
    }
    else{
        vote--;
    }
    if (train_l[1]){
        vote++;
    }
    else{
        vote--;
    }
    if (train_l[2]){
        vote++;
    }
    else{
        vote--;
    }

    if (vote > 0){
        return 1;
    }
    else{
        return 0;
    }
}

