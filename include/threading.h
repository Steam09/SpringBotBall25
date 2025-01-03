#ifndef THREADING_H
#define THREADING_H

#include <kipr/wombat.h>

// Thread function declarations
void start_movement_threads();
void stop_movement_threads();

// Movement control thread functions
int forward_thread();
int turns_thread();
int sensor_monitor_thread();

// Thread management
extern thread* move_thread;
extern thread* turn_thread;
extern thread* sensor_thread;

// Thread control flags
extern int thread_running;
extern int emergency_stop;

#endif // THREADING_H
