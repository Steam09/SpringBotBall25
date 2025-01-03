#include <kipr/motor/motor.h>
#include <kipr/servo/servo.h>
#include <kipr/wombat.h>
#include "../include/variables.h"
#include "../include/movement.h"

float boardAngle = 0;

/** STOP THE ROBOT */
void STOP() {
    freeze(wheelPorts.frontLeft);
    freeze(wheelPorts.frontRight);
    freeze(wheelPorts.backLeft);
    freeze(wheelPorts.backRight);
    disable_servos();
    msleep(10);
    enable_servos();
}

/** Allows the robot to drive forwards at speed (speed) and distance in cm */
void forwardDrive(float distanceCM, int speed) {
    mav(wheelPorts.frontLeft, speed);
    mav(wheelPorts.frontRight, speed);
    mav(wheelPorts.backLeft, speed);
    mav(wheelPorts.backRight, speed);
    msleep(2000);  // Run for 2 seconds
    STOP(); // All off
    boardAngle = boardAngle; // It should stay the same for this movement
}

/** Allows the robot to drive to the left at speed (speed) and distance in cm */
void leftDrive(float distanceCM, int speed) {
    mav(wheelPorts.frontLeft, speed);
    mav(wheelPorts.frontRight, (-1) * speed);
    mav(wheelPorts.backLeft, (-1) * speed);
    mav(wheelPorts.backRight, speed);
    msleep(2000);  // Run for 2 seconds
    STOP();  // All off
    boardAngle = boardAngle; // It should stay the same for this movement
}

/** Allows the robot to drive to the right at speed (speed) and distance in cm */
void rightDrive(float distanceCM, int speed) {
    mav(wheelPorts.frontLeft, (-1) * speed);
    mav(wheelPorts.frontRight, speed);
    mav(wheelPorts.backLeft, speed);
    mav(wheelPorts.backRight, (-1) * speed);
    msleep(2000);  // Run for 2 seconds
    ao();  // All off
    boardAngle = boardAngle; // It should stay the same for this movement
}

/** Allows the robot to drive backwards at speed (speed) and distance in cm */
void backDrive(float distanceCM, int speed) {
    mav(wheelPorts.frontLeft, (-1) * speed);
    mav(wheelPorts.frontRight, (-1) * speed);
    mav(wheelPorts.backLeft, (-1) * speed);
    mav(wheelPorts.backRight, (-1) * speed);
    msleep(2000);  // Run for 2 seconds
    ao();  // All off
    boardAngle = boardAngle; // It should stay the same for this movement
}

void lower_servo_smoothly(int servo_port, int target_position, int step_size, int delay_ms) {
    int current_position = get_servo_position(servo_port);

    if (current_position > target_position) {
        // Moving down
        while (current_position > target_position) {
            current_position -= step_size;
            if (current_position < target_position) {
                current_position = target_position;
            }
            set_servo_position(servo_port, current_position);
            msleep(delay_ms);
        }
    } else {
        // Moving up
        while (current_position < target_position) {
            current_position += step_size;
            if (current_position > target_position) {
                current_position = target_position;
            }
            set_servo_position(servo_port, current_position);
            msleep(delay_ms);
        }
    }
}
