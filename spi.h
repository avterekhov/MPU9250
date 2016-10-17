/**
 * Invensense MPU-9250 SPI Library for BeagleBone
 *
 * Copyright (C) 2016 Alexander V. Terekhov
 *
 * Open source under the MIT License. See LICENSE.txt.
 */

#ifndef __SPI_H
#define __SPI_H

#include <stdint.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

int spi_open(const char* filename);
void spi_close(int file);
uint8_t transfer_2bytes(int file, uint8_t byte1, uint8_t byte2);
void transfer_buffer(int file, uint8_t command, uint8_t* buffer, int size);

#endif
