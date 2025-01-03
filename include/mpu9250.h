#ifndef MPU9250_H
#define MPU9250_H

typedef struct {
    float x;
    float y;
    float z;
} Vector3;

void init_mpu9250();
Vector3 get_acceleration();
Vector3 get_gyro();
void print_sensor_data();
void get_orientation(float* pitch, float* roll);

#endif
