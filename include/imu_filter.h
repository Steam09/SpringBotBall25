#ifndef IMU_FILTER_H
#define IMU_FILTER_H

#include "mpu9250.h"

typedef struct {
    float roll;    // degrees
    float pitch;   // degrees
    float yaw;     // degrees
} orientation_t;

void filter_init(void);
void update_filter(mpu9250_data_t *data, orientation_t *orientation);

#endif
