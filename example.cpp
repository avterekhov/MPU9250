#include <stdio.h>
#include <unistd.h>
#include "spi.h"
#include "MPU9250.h"

int main(void)
{
  int spi_file = spi_open("/dev/spidev1.0");
  MPU9250 mpu(spi_file);
  printf("WhoAmI: %.2X\n", mpu.whoami());

  mpu.init(0,0);
  printf("Accelerometers\t\t\tGyroscopes\n");
  for(int tmp = 0; tmp < 1000; tmp++) {
    mpu.read_acc();
    printf("% 1.4f, % 1.4f, % 1.4f\t", mpu.accel_data[0], mpu.accel_data[1], mpu.accel_data[2]);
    mpu.read_gyro();
    printf("% 1.4f, % 1.4f, % 1.4f\n", mpu.gyro_data[0], mpu.gyro_data[1], mpu.gyro_data[2]);
    usleep(50000);
    }
  spi_close(spi_file);
  return 0;
}
