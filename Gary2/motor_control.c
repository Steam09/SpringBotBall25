#include "../include/motor_control.h"
#include <kipr/wombat.h>

#define NUM_MOTORS 4
#define CALIBRATION_TIME 5000  // 5 seconds
#define PID_INTERVAL 10  // 10ms

typedef struct {
    float kp;
    float ki;
    float kd;
    float previous_error;
    float integral;
    float max_speed;
    int port;
} motor_config_t;

static motor_config_t motors[NUM_MOTORS];

void motor_init(void) {
    // Initialize motor configurations
    for(int i = 0; i < NUM_MOTORS; i++) {
        motors[i].kp = 0.5f;
        motors[i].ki = 0.1f;
        motors[i].kd = 0.05f;
        motors[i].previous_error = 0.0f;
        motors[i].integral = 0.0f;
        motors[i].max_speed = 0.0f;
        motors[i].port = i;  // Assuming motors are on ports 0-3
    }
}

void motor_calibrate(void) {
    printf("Starting motor calibration...\n");
    printf("Each motor will run at full power for 5 seconds\n");

    for(int i = 0; i < NUM_MOTORS; i++) {
        clear_motor_position_counter(motors[i].port);

        // Run motor at full power
        motor(motors[i].port, 100);
        int start_time = systime();
        int start_position = get_motor_position_counter(motors[i].port);

        msleep(CALIBRATION_TIME);

        // Stop motor and calculate max speed
        motor(motors[i].port, 0);
        int end_position = get_motor_position_counter(motors[i].port);
        float ticks_per_second = (float)(end_position - start_position) /
                                (CALIBRATION_TIME / 1000.0f);

        motors[i].max_speed = ticks_per_second;

        printf("Motor %d max speed: %.2f ticks/second\n",
               i, motors[i].max_speed);

        msleep(1000);  // Cool down period
    }
}

void motor_pid_update(int motor_num, float target_speed) {
    if(motor_num >= NUM_MOTORS) return;

    static int last_time = 0;
    int current_time = systime();

    // Ensure consistent update interval
    if(current_time - last_time < PID_INTERVAL) return;

    // Calculate current speed
    static int last_position[NUM_MOTORS] = {0};
    int current_position = get_motor_position_counter(motors[motor_num].port);
    float current_speed = (float)(current_position - last_position[motor_num]) /
                         (PID_INTERVAL / 1000.0f);

    // Calculate error
    float error = target_speed - current_speed;

    // Update integral
    motors[motor_num].integral += error * (PID_INTERVAL / 1000.0f);

    // Calculate derivative
    float derivative = (error - motors[motor_num].previous_error) /
                      (PID_INTERVAL / 1000.0f);

    // Calculate PID output
    float output = motors[motor_num].kp * error +
                  motors[motor_num].ki * motors[motor_num].integral +
                  motors[motor_num].kd * derivative;

    // Convert to motor power (-100 to 100)
    int power = (int)((output / motors[motor_num].max_speed) * 100.0f);
    if(power > 100) power = 100;
    if(power < -100) power = -100;

    // Apply motor power
    motor(motors[motor_num].port, power);

    // Update state for next iteration
    motors[motor_num].previous_error = error;
    last_position[motor_num] = current_position;
    last_time = current_time;
}
