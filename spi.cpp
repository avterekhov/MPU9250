/**
 * Invensense MPU-9250 SPI Library for BeagleBone
 *
 * Copyright (C) 2016 Alexander V. Terekhov
 *
 * Open source under the MIT License. See LICENSE.txt.
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "spi.h"

static uint8_t mode = 0;
static uint8_t bits = 8;
static uint32_t speed = 500000;
static uint16_t delay = 0;

void pabort(const char *s)
{
  perror(s);
  abort();
}


uint8_t transfer_2bytes(int file, uint8_t byte1, uint8_t byte2)
{
  int ret;
  uint8_t tx[] = {
    byte1, byte2
  };
  uint8_t rx[ARRAY_SIZE(tx)] = {0, };
  struct spi_ioc_transfer tr;
  tr.tx_buf = (unsigned long)tx;
  tr.rx_buf = (unsigned long)rx;
  tr.len = ARRAY_SIZE(tx);
  tr.delay_usecs = delay;
  tr.speed_hz = speed;
  tr.bits_per_word = bits;
  ret = ioctl(file, SPI_IOC_MESSAGE(1), &tr);
  if (ret < 1)
    pabort("can't send spi message");
  return rx[1];
}

void transfer_buffer(int file, uint8_t command, uint8_t* buffer, int size)
{
  int ret;
  uint8_t tx[255] = {command};
  uint8_t* rx = buffer;
  struct spi_ioc_transfer tr;
  tr.tx_buf = (unsigned long)tx;
  tr.rx_buf = (unsigned long)rx;
  tr.len = size+1;
  tr.delay_usecs = delay;
  tr.speed_hz = speed;
  tr.bits_per_word = bits;
  ret = ioctl(file, SPI_IOC_MESSAGE(1), &tr);
  if (ret < 1)
    pabort("can't send spi message");
}

int spi_open(const char* device)
{
  int ret = 0;
  int fd = open(device, O_RDWR);
  if (fd < 0)
    pabort("can't open device");

  /*
   * spi mode
   */
  ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
  if (ret == -1)
    pabort("can't set spi mode");

  ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
  if (ret == -1)
    pabort("can't get spi mode");

  /*
   * bits per word
   */
  ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
  if (ret == -1)
    pabort("can't set bits per word");

  ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
  if (ret == -1)
    pabort("can't get bits per word");

  /*
   * max speed hz
   */
  ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
  if (ret == -1)
    pabort("can't set max speed hz");

  ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
  if (ret == -1)
    pabort("can't get max speed hz");

  printf("spi mode: %d\n", mode);
  printf("bits per word: %d\n", bits);
  printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);

  return fd;
}

void spi_close(int spi_file)
{
  close(spi_file);
}
