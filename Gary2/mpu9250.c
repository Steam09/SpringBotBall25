#include "../include/mpu9250.h"
#include "../include/i2c_comm.h"
#include <math.h>
#include <kipr/wombat.h>

#define GYRO_SCALE (250.0/32768.0)    // ±250 deg/s
#define ACCEL_SCALE (2.0/32768.0)     // ±2g
#define MAG_SCALE (4912.0/32760.0)    // ±4912 µT

static float gyro_offset[3] = {0};
static float accel_offset[3] = {0};
static float mag_offset[3] = {0};
static float mag_scale[3] = {1, 1, 1};

void mpu9250_init(void) {
    // Wake up MPU9250
    i2c_write_byte(MPU9250_ADDR, 0x6B, 0x00);
    msleep(100);

    // Configure gyroscope (±250dps)
    i2c_write_byte(MPU9250_ADDR, 0x1B, 0x00);

    // Configure accelerometer (±2g)
    i2c_write_byte(MPU9250_ADDR, 0x1C, 0x00);

    // Enable bypass mode for magnetometer access
    i2c_write_byte(MPU9250_ADDR, 0x37, 0x02);
    msleep(100);

    // Initialize magnetometer
    i2c_write_byte(MAG_ADDR, 0x0A, 0x16); // 16-bit resolution, 100Hz
}

void mpu9250_calibrate(void) {
    const int num_samples = 1000;
    float gyro_sum[3] = {0};
    float accel_sum[3] = {0};
    float mag_min[3] = {1e6, 1e6, 1e6};
    float mag_max[3] = {-1e6, -1e6, -1e6};

    printf("Keep the sensor still for gyro calibration...\n");
    msleep(2000);

    for(int i = 0; i < num_samples; i++) {
        mpu9250_data_t data;
        mpu9250_read_raw(&data);

        for(int j = 0; j < 3; j++) {
            gyro_sum[j] += data.gyro[j];
            accel_sum[j] += data.accel[j];

            // Update magnetometer min/max
            mag_min[j] = fmin(mag_min[j], data.mag[j]);
            mag_max[j] = fmax(mag_max[j], data.mag[j]);
        }
        msleep(2);
    }

    // Calculate offsets
    for(int i = 0; i < 3; i++) {
        gyro_offset[i] = gyro_sum[i] / num_samples;
        accel_offset[i] = accel_sum[i] / num_samples;
        if(i == 2) accel_offset[i] -= 1.0f; // Remove gravity

        // Calculate magnetometer scale and offset
        mag_offset[i] = (mag_min[i] + mag_max[i]) / 2.0f;
        mag_scale[i] = (mag_max[i] - mag_min[i]) / 2.0f;
    }

    printf("Calibration complete!\n");
}

void mpu9250_read_raw(mpu9250_data_t *data) {
    unsigned char buf[14];

    // Read accelerometer and gyroscope
    i2c_read_bytes(MPU9250_ADDR, 0x3B, buf, 14);

    // Convert accelerometer data
    for(int i = 0; i < 3; i++) {
        __int16_t raw = (buf[i*2] << 8) | buf[i*2 + 1];
        data->accel[i] = ((float)raw) * ACCEL_SCALE;
    }

    // Convert gyroscope data
    for(int i = 0; i < 3; i++) {
        __int16_t raw = (buf[i*2 + 8] << 8) | buf[i*2 + 9];
        data->gyro[i] = ((float)raw) * GYRO_SCALE;
    }

    // Read magnetometer
    unsigned char mag_buf[7];
    i2c_read_bytes(MAG_ADDR, 0x03, mag_buf, 7);

    // Convert magnetometer data
    for(int i = 0; i < 3; i++) {
        __int16_t raw = (mag_buf[i*2] << 8) | mag_buf[i*2 + 1];
        data->mag[i] = ((float)raw) * MAG_SCALE;
    }
}

void mpu9250_read_data(mpu9250_data_t *data) {
    mpu9250_read_raw(data);

    // Apply calibration
    for(int i = 0; i < 3; i++) {
        data->gyro[i] -= gyro_offset[i];
        data->accel[i] -= accel_offset[i];
        data->mag[i] = (data->mag[i] - mag_offset[i]) / mag_scale[i];
    }
}
