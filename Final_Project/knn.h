/*
 * knn.h
 *
 *  Created on: Dec 6, 2020
 *      Author: Samuel
 */

#ifndef KNN_H_
#define KNN_H_

#include <stdint.h>
#include <math.h>
#include "GPIOlib.h"

/***************** Training Defines *****************/

#define train_size  40                      // 40 Samples that we are training on
#define mel_len     101                     // Len of each sample's mel
#define array_size  (train_size * mel_len)  // Total size of train array

/***************** Function Prototype *****************/

uint16_t knn(float* test_d, float* train_d, uint16_t* train_l);

#endif /* KNN_H_ */
