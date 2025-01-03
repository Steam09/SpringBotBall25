#include "../include/fastgyro.h"

float FastGyro::readings[FILTER_SIZE] = {0};
int FastGyro::readIndex = 0;
float FastGyro::zeroOffset = 0;

float FastGyro::movingAverage(float newReading) {
    readings[readIndex] = newReading;
    readIndex = (readIndex + 1) % FILTER_SIZE;

    float sum = 0;
    for(int i = 0; i < FILTER_SIZE; i++) {
        sum += readings[i];
    }
    return sum / FILTER_SIZE;
}

void FastGyro::init() {
    i2c_init();
    i2c_write8(MPU9250_ADDRESS, 0x6B, 0x00);
    i2c_write8(MPU9250_ADDRESS, GYRO_CONFIG, 0x00);
    i2c_write8(MPU9250_ADDRESS, SMPLRT_DIV, 0x07);
    i2c_write8(MPU9250_ADDRESS, CONFIG, 0x03);
}

void FastGyro::calibrate() {
    float sum = 0;
    const int samples = 600;

    for(int i = 0; i < samples; i++) {
        short raw_z = (i2c_read8(MPU9250_ADDRESS, GYRO_Z_H) << 8) |
                      i2c_read8(MPU9250_ADDRESS, GYRO_Z_L);
        sum += raw_z / 131.0f;
        msleep(10);
    }
    zeroOffset = sum / samples;
}

float FastGyro::readGyroZ() {
    short raw_z = (i2c_read8(MPU9250_ADDRESS, GYRO_Z_H) << 8) |
                  i2c_read8(MPU9250_ADDRESS, GYRO_Z_L);
    float currentReading = (raw_z / 131.0f) - zeroOffset;
    return movingAverage(currentReading);
}
