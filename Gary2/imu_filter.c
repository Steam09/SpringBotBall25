#include "../include/imu_filter.h"
#include <math.h>
#include <kipr/wombat.h>

#define PI 3.14159265359f
#define COMPLEMENTARY_ALPHA 0.96f
#define DT 0.01f  // 10ms sample time

static orientation_t current_orientation = {0};

void filter_init(void) {
    current_orientation.roll = 0.0f;
    current_orientation.pitch = 0.0f;
    current_orientation.yaw = 0.0f;
}

static void normalize_vector(float *vector) {
    float magnitude = sqrt(vector[0] * vector[0] +
                         vector[1] * vector[1] +
                         vector[2] * vector[2]);
    if (magnitude != 0) {
        vector[0] /= magnitude;
        vector[1] /= magnitude;
        vector[2] /= magnitude;
    }
}

void update_filter(mpu9250_data_t *data, orientation_t *orientation) {
    // Calculate pitch and roll from accelerometer
    float accel_roll = atan2f(data->accel[1], data->accel[2]) * 180.0f / PI;
    float accel_pitch = atan2f(-data->accel[0],
                              sqrt(data->accel[1] * data->accel[1] +
                                   data->accel[2] * data->accel[2])) * 180.0f / PI;

    // Integrate gyroscope data
    orientation->roll = COMPLEMENTARY_ALPHA * (orientation->roll +
                       data->gyro[0] * DT) +
                       (1.0f - COMPLEMENTARY_ALPHA) * accel_roll;

    orientation->pitch = COMPLEMENTARY_ALPHA * (orientation->pitch +
                        data->gyro[1] * DT) +
                        (1.0f - COMPLEMENTARY_ALPHA) * accel_pitch;

    // Calculate yaw using magnetometer
    float mag_x = data->mag[0] * cosf(orientation->pitch * PI / 180.0f) +
                 data->mag[2] * sinf(orientation->pitch * PI / 180.0f);

    float mag_y = data->mag[0] * sinf(orientation->roll * PI / 180.0f) *
                 sinf(orientation->pitch * PI / 180.0f) +
                 data->mag[1] * cosf(orientation->roll * PI / 180.0f) -
                 data->mag[2] * sinf(orientation->roll * PI / 180.0f) *
                 cosf(orientation->pitch * PI / 180.0f);

    float yaw = atan2f(-mag_y, mag_x) * 180.0f / PI;

    // Integrate gyroscope yaw with magnetometer yaw
    orientation->yaw = COMPLEMENTARY_ALPHA * (orientation->yaw +
                      data->gyro[2] * DT) +
                      (1.0f - COMPLEMENTARY_ALPHA) * yaw;

    // Normalize yaw to 0-360 degrees
    if (orientation->yaw < 0) orientation->yaw += 360.0f;
    else if (orientation->yaw > 360.0f) orientation->yaw -= 360.0f;
}
