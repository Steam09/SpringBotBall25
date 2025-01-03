#ifndef FASTGYRO_HPP
#define FASTGYRO_HPP

#include <kipr/wombat.h>
#include "i2cHeaders.h"

// I2C function declarations
int i2c_read8(int address, int reg);
int i2c_write8(int address, int reg, int value);
void i2c_init();

struct FastGyro {
    private:
        static const unsigned char MPU9250_ADDRESS = 0x68;
        static const unsigned char GYRO_CONFIG = 0x1B;
        static const unsigned char SMPLRT_DIV = 0x19;
        static const unsigned char CONFIG = 0x1A;
        static const unsigned char GYRO_Z_H = 0x47;
        static const unsigned char GYRO_Z_L = 0x48;

        static const int FILTER_SIZE = 300;
        static float readings[FILTER_SIZE];
        static int readIndex;
        static float zeroOffset;

        static float movingAverage(float newReading);

    public:
        static void init();
        static void calibrate();
        static float readGyroZ();
};

#endif
