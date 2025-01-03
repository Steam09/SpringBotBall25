#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

void motor_init(void);
void motor_calibrate(void);
void motor_pid_update(int motor_num, float target_speed);

#endif
