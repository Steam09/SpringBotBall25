#include <kipr/graphics/graphics_key_code.h>
#include <kipr/wombat.h>
#include <stdint.h>
#include "../include/library.h"
#undef get_acceleration

int main() {
    checks();






    return 0;
}
/*
int threadingPrac() {

    printf("Starting threading test...\n");

    // Initialize systems
    init_mpu9250();

    // Start threads
    start_movement_threads();

    // Main program loop
    double start_time = seconds();
    while(seconds() - start_time < 5 && !emergency_stop) {
        printf("Main thread running... Time elapsed: %f seconds\n",
               seconds() - start_time);
        msleep(500);  // Print status every 0.5 seconds
    }

    // Stop threads
    stop_movement_threads();

    // Stop all motors
    ao();

    printf("Threading test complete\n");
    return 0;
}
*/
void testMPU() {

    printf("Initializing MPU9250...\n");
        init_mpu9250();
        msleep(100);  // Give the sensor time to initialize

        printf("Beginning sensor readings...\n");

        // Read sensor data for 10 seconds
        int start_time = seconds();
        float pitchAvg = 0;
        float rollAvg = 0;
        int i = 0;
        while(seconds() - start_time < 10) {
            print_sensor_data();

            float pitch, roll;
            get_orientation(&pitch, &roll);
            printf("Orientation - Pitch: %.2f° Roll: %.2f°\n", pitch, roll);
            pitchAvg += pitch;
            rollAvg += roll;
            i += 1;
            printf("------------------------\n");
            msleep(50);  // Update every 50ms
        }
        printf("Avg Orientation - Pitch: %.2f° Roll: %.2f°\n", pitchAvg / i, rollAvg / i);
}

void driveTest() {
    int distance = 5.000;
    forwardDrive(distance, 1500);
    rightDrive(distance, 1500);
    backDrive(distance, 1500);
    leftDrive(distance, 1500);
}
/*
void realPID() {
    printf("Initializing MPU9250...\n");
    init_mpu9250();
    msleep(100);

    // Test sequence
    drive_straight_pid(50.0, 1500);  // Drive forward 50cm
    msleep(500);

    rotate_to_angle_pid(90.0, 1000);  // Rotate 90 degrees
    msleep(500);

    strafe_pid(30.0, 1500, 1);  // Strafe right 30cm
    msleep(500);

    rotate_to_angle_pid(0.0, 1000);  // Return to original orientation

}

void testPID() {
    printf("Starting PID tuning...\n");
    init_mpu9250();
    msleep(100);

    while (!a_button()) {  // Run until A button is pressed
        // Test straight line motion
        printf("Testing straight line motion...\n");
        drive_straight_pid(50.0, 1000);  // 50cm at 1000 speed
        msleep(1000);

        // Test rotation
        printf("Testing rotation...\n");
        rotate_to_angle_pid(90.0, 800);
        msleep(1000);
        rotate_to_angle_pid(0.0, 800);
        msleep(1000);

        // Test strafing
        printf("Testing strafing...\n");
        strafe_pid(30.0, 1000, 1);  // Right
        msleep(1000);
        strafe_pid(30.0, 1000, -1); // Left
        msleep(1000);

        printf("Press A to stop, B to repeat\n");
        msleep(500);
    }
}
*/
