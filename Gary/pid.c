#include <kipr/wombat.h>
#include "../include/pid.h"
#include "../include/variables.h"
#include "../include/mpu9250.h"
#include "../include/movement.h"
#include <math.h>

void pid_init(PIDController* pid, float kP, float kI, float kD, float minOutput, float maxOutput) {
    pid->kP = kP;
    pid->kI = kI;
    pid->kD = kD;
    pid->lastError = 0;
    pid->integral = 0;
    pid->minOutput = minOutput;
    pid->maxOutput = maxOutput;
}



float pid_compute(PIDController* pid, float current, float dt) {
    float error = pid->target - current;

    // Proportional term
    float P = pid->kP * error;

    // Integral term
    pid->integral += error * dt;
    float I = pid->kI * pid->integral;

    // Derivative term
    float derivative = (error - pid->lastError) / dt;
    float D = pid->kD * derivative;

    pid->lastError = error;

    // Calculate total output
    float output = P + I + D;

    // Clamp output to limits
    if (output > pid->maxOutput) output = pid->maxOutput;
    if (output < pid->minOutput) output = pid->minOutput;

    return output;
}

void pid_reset(PIDController* pid) {
    pid->lastError = 0;
    pid->integral = 0;
}

// Global variables for orientation tracking
static float currentAngle = 0;
static float lastGyroZ = 0;
static unsigned long lastTime = 0;

void debug_pid_values(PIDController* pid, float current) {
    float error = pid->target - current;
    printf("Target: %.2f, Current: %.2f, Error: %.2f\n",
           pid->target, current, error);
    printf("P: %.2f, I: %.2f, D: %.2f\n",
           pid->kP * error,
           pid->kI * pid->integral,
           pid->kD * (error - pid->lastError));
    msleep(50);
}

void update_angle() {
    Vector3 gyro = get_gyro();
    unsigned long currentTime = systime();
    float dt = (currentTime - lastTime) / 1000.0f; // Convert to seconds

    if (dt > 0) {
        // Simple gyro integration
        currentAngle += ((gyro.z + lastGyroZ) / 2.0f) * dt;

        // Keep angle between -180 and 180
        while (currentAngle > 180) currentAngle -= 360;
        while (currentAngle < -180) currentAngle += 360;
    }

    lastGyroZ = gyro.z;
    lastTime = currentTime;
}

void drive_straight_pid(float distanceCM, int baseSpeed) {
    PIDController anglePID;
    // Modify these values (kP, kI, kD, minOutput, maxOutput)
    pid_init(&anglePID, 1.0, 0.0, 0.1, -500, 500); // Tune these values

    // Store initial angle as target
    anglePID.target = currentAngle;

    // Calculate time needed based on distance and speed
    float timeNeeded = (distanceCM / (baseSpeed * 0.01)) * 1000; // Rough estimation
    unsigned long startTime = systime();

    while ((systime() - startTime) < timeNeeded) {
        update_angle();
        float correction = pid_compute(&anglePID, currentAngle, 0.02);

        // Debug output
        debug_pid_values(&anglePID, currentAngle);

        // Apply speeds to wheels with correction
        mav(wheelPorts.frontLeft, baseSpeed + correction);
        mav(wheelPorts.frontRight, baseSpeed - correction);
        mav(wheelPorts.backLeft, baseSpeed + correction);
        mav(wheelPorts.backRight, baseSpeed - correction);

        msleep(20);
    }

    STOP();
}

void strafe_pid(float distanceCM, int baseSpeed, int direction) {
    PIDController anglePID;
    // Modify these values (kP, kI, kD, minOutput, maxOutput)
    pid_init(&anglePID, 1.5, 0.0, 0.15, -500, 500);

    anglePID.target = currentAngle;

    float timeNeeded = (distanceCM / (baseSpeed * 0.01)) * 1000;
    unsigned long startTime = systime();

    while ((systime() - startTime) < timeNeeded) {
        update_angle();
        float correction = pid_compute(&anglePID, currentAngle, 0.02);

        // Mecanum wheel strafing with correction
        int frontLeft = direction * baseSpeed + correction;
        int frontRight = -direction * baseSpeed - correction;
        int backLeft = -direction * baseSpeed + correction;
        int backRight = direction * baseSpeed - correction;

        mav(wheelPorts.frontLeft, frontLeft);
        mav(wheelPorts.frontRight, frontRight);
        mav(wheelPorts.backLeft, backLeft);
        mav(wheelPorts.backRight, backRight);

        msleep(20);
    }

    STOP();
}

void rotate_to_angle_pid(float targetAngle, int baseSpeed) {
    PIDController rotationPID;
    // Modify these values (kP, kI, kD, minOutput, maxOutput)
    pid_init(&rotationPID, 2.0, 0.0, 0.2, -baseSpeed, baseSpeed);

    rotationPID.target = targetAngle;

    float angleError;
    do {
        update_angle();
        float rotationSpeed = pid_compute(&rotationPID, currentAngle, 0.02);

        mav(wheelPorts.frontLeft, rotationSpeed);
        mav(wheelPorts.frontRight, -rotationSpeed);
        mav(wheelPorts.backLeft, rotationSpeed);
        mav(wheelPorts.backRight, -rotationSpeed);

        angleError = fabs(targetAngle - currentAngle);
        msleep(20);
    } while (angleError > 2.0); // 2-degree tolerance

    STOP();
}
