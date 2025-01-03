#include "../include/library.h"
#include <signal.h>

static volatile int running = 1;

void signal_handler(int signum) {
    running = 0;
}

int main() {
    printf("debug 1\n");
    printf("debug 2\n");
    printf("debug 3\n");
    printf("debug 4\n");
    msleep(50000);
    // Set up signal handler for clean shutdown
    signal(SIGINT, signal_handler);

    // Initialize all systems
    printf("Initializing systems...\n");
    i2c_init();
    mpu9250_init();
    filter_init();
    motor_init();

    // Perform calibration
    printf("Beginning calibration sequence...\n");
    printf("Keep the robot still for sensor calibration\n");
    mpu9250_calibrate();

    printf("Beginning motor calibration\n");
    printf("Robot will move during this process\n");
    motor_calibrate();

    mpu9250_data_t sensor_data;
    orientation_t orientation = {0};
    float target_speed = 360.0f; // 1 rotation per second

    printf("Starting main control loop\n");
    while(running) {
        // Read sensor data
        mpu9250_read_data(&sensor_data);

        // Update orientation estimation
        update_filter(&sensor_data, &orientation);

        // Update motor control
        for(int i = 0; i < 4; i++) {
            motor_pid_update(i, target_speed);
        }

        // Print debug information every 500ms
        static int last_print = 0;
        int current_time = systime();
        if(current_time - last_print > 500) {
            printf("Orientation (deg) - Roll: %.2f, Pitch: %.2f, Yaw: %.2f\n",
                   orientation.roll, orientation.pitch, orientation.yaw);
            last_print = current_time;
        }

        msleep(10); // 100Hz update rate
    }

    // Clean shutdown
    for(int i = 0; i < 4; i++) {
        motor(i, 0);
    }

    printf("Program terminated\n");
    return 0;
}
