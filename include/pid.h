#ifndef PID_H
#define PID_H

typedef struct {
    float kP;
    float kI;
    float kD;
    float lastError;
    float integral;
    float target;
    float minOutput;
    float maxOutput;
} PIDController;

void pid_init(PIDController* pid, float kP, float kI, float kD, float minOutput, float maxOutput);
float pid_compute(PIDController* pid, float current, float dt);
void pid_reset(PIDController* pid);
void debug_pid_values(PIDController* pid, float current);

// Movement functions with PID control
void drive_straight_pid(float distanceCM, int baseSpeed);
void strafe_pid(float distanceCM, int baseSpeed, int direction); // 1 for right, -1 for left
void rotate_to_angle_pid(float targetAngle, int baseSpeed);

#endif
