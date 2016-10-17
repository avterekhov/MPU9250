example: spi.o MPU9250.o
	gcc -o example example.cpp spi.o MPU9250.o
spi.o: spi.*
	gcc -o spi.o -c spi.cpp
MPU9250.o: MPU9250.* spi.h
	gcc -o MPU9250.o -c MPU9250.cpp
