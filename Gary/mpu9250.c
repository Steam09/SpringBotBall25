#include <stdint.h>
#include <kipr/wombat.h>
#include "../include/i2cHeaders.h"
#include "../include/mpu9250.h"
#include <stdio.h>
#include <math.h>

// MPU-9250 registers
#define MPU9250_ADDR      0x68
#define PWR_MGMT_1        0x6B
#define CONFIG            0x1A
#define GYRO_CONFIG       0x1B
#define ACCEL_CONFIG      0x1C
#define ACCEL_CONFIG2     0x1D
#define ACCEL_XOUT_H      0x3B
#define GYRO_XOUT_H       0x43
#define WHO_AM_I          0x75

// Sensor scale settings
#define GYRO_SCALE_250DPS 0x00
#define GYRO_SCALE_500DPS 0x08
#define GYRO_SCALE_1000DPS 0x10
#define GYRO_SCALE_2000DPS 0x18

#define ACCEL_SCALE_2G    0x00
#define ACCEL_SCALE_4G    0x08
#define ACCEL_SCALE_8G    0x10
#define ACCEL_SCALE_16G   0x18

// Scale modifiers
#define ACCEL_SCALE_MODIFIER_2G  16384.0
#define GYRO_SCALE_MODIFIER_250  131.0

void init_mpu9250() {
    i2c_init();

    // Check if sensor is responsive
    int who_am_i = i2c_read8(MPU9250_ADDR, WHO_AM_I);
    if (who_am_i != 0x71) {  // MPU9250 should return 0x71
        printf("Error: MPU9250 not detected! WHO_AM_I = 0x%02X\n", who_am_i);
        return;
    }

    // Wake up the sensor (clear sleep bit)
    i2c_write8(MPU9250_ADDR, PWR_MGMT_1, 0x00);
    msleep(100);  // Wait for sensor to stabilize

    // Configure accelerometer (±2g range)
    i2c_write8(MPU9250_ADDR, ACCEL_CONFIG, ACCEL_SCALE_2G);

    // Configure gyroscope (±250 degrees/second range)
    i2c_write8(MPU9250_ADDR, GYRO_CONFIG, GYRO_SCALE_250DPS);

    printf("MPU9250 initialized successfully!\n");
}

__int16_t read_raw_data(int reg) {
    int high = i2c_read8(MPU9250_ADDR, reg);
    int low = i2c_read8(MPU9250_ADDR, reg + 1);
    return (high << 8) | low;
}

Vector3 get_acceleration() {
    Vector3 accel;
    accel.x = read_raw_data(ACCEL_XOUT_H) / ACCEL_SCALE_MODIFIER_2G;
    accel.y = read_raw_data(ACCEL_XOUT_H + 2) / ACCEL_SCALE_MODIFIER_2G;
    accel.z = read_raw_data(ACCEL_XOUT_H + 4) / ACCEL_SCALE_MODIFIER_2G;
    return accel;
}

Vector3 get_gyro() {
    Vector3 gyro;
    gyro.x = read_raw_data(GYRO_XOUT_H) / GYRO_SCALE_MODIFIER_250;
    gyro.y = read_raw_data(GYRO_XOUT_H + 2) / GYRO_SCALE_MODIFIER_250;
    gyro.z = read_raw_data(GYRO_XOUT_H + 4) / GYRO_SCALE_MODIFIER_250;
    return gyro;
}

void print_sensor_data() {
    Vector3 accel = get_acceleration();
    Vector3 gyro = get_gyro();

    printf("Acceleration (g): X=%.2f Y=%.2f Z=%.2f\n", accel.x, accel.y, accel.z);
    printf("Gyro (deg/s): X=%.2f Y=%.2f Z=%.2f\n", gyro.x, gyro.y, gyro.z);
}

// Calculate pitch and roll angles from accelerometer data
void get_orientation(float* pitch, float* roll) {
    Vector3 accel = get_acceleration();

    // Calculate pitch and roll in degrees
    *pitch = atan2(-accel.x, sqrt(accel.y * accel.y + accel.z * accel.z)) * 180.0 / M_PI;
    *roll = atan2(accel.y, accel.z) * 180.0 / M_PI;
}
