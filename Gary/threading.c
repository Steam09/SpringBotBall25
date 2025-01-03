#include <kipr/wombat.h>
#include "../include/threading.h"
// #include "../include/variables.h"
#include <stdio.h>

// Thread handles
thread* move_thread = NULL;
thread* turn_thread = NULL;
thread* sensor_thread = NULL;

// Control flags
int thread_running = 1;
int emergency_stop = 0;

// Mutex for thread synchronization
mutex movement_mutex;
/*
void start_movement_threads() {
    // Initialize mutex
    mutex_create(&movement_mutex);

    printf("Starting threads...\n");
    thread_running = 1;
    emergency_stop = 0;

    // Start individual threads with proper type casting with error checking
    sensor_thread = thread_create(sensor_monitor_thread);
    if (sensor_thread == NULL) {
        printf("Failed to create sensor thread!\n");
        return;
    }

    move_thread = thread_create(forward_thread);
    if (move_thread == NULL) {
        printf("Failed to create movement thread!\n");
        thread_destroy(*sensor_thread);
        return;
    }

    turn_thread = thread_create(turns_thread);
    if (turn_thread == NULL) {
        printf("Failed to create turn thread!\n");
        thread_destroy(*sensor_thread);
        thread_destroy(*move_thread);
        return;
    }

    if (!sensor_thread || !move_thread || !turn_thread) {
        printf("Error: Failed to create one or more threads\n");
        return;
    }

    printf("Threads started successfully\n");
}

void stop_movement_threads() {
    // Signal threads to stop
    thread_running = 0;

    // Wait for threads to finish
    thread_destroy(*sensor_thread);
    thread_destroy(*move_thread);
    thread_destroy(*turn_thread);

    // Destroy mutex
    mutex_destroy(movement_mutex);

    printf("All threads stopped\n");
}

// Thread function for forward movement
int forward_thread() {
    while(thread_running && !emergency_stop) {
        // Lock mutex before accessing shared resources
        mutex_lock(movement_mutex);

        // Basic forward movement
        mav(wheelPorts.frontLeft, 1000);
        mav(wheelPorts.frontRight, 1000);
        mav(wheelPorts.backLeft, 1000);
        mav(wheelPorts.backRight, 1000);

        // Unlock mutex
        mutex_unlock(movement_mutex);

        // Sleep to prevent thread from hogging CPU
        msleep(20);
    }
    return 0;
}

// Thread function for turning
int turns_thread() {
    while(thread_running && !emergency_stop) {
        mutex_lock(movement_mutex);

        // Example: Periodic slight turn adjustment
        mav(wheelPorts.frontLeft, 1050);
        mav(wheelPorts.frontRight, 950);
        mav(wheelPorts.backLeft, 1050);
        mav(wheelPorts.backRight, 950);

        mutex_unlock(movement_mutex);
        msleep(20);
    }
    return 0;
}

// Thread for monitoring sensors
int sensor_monitor_thread() {
    while(thread_running) {
        // Example: Check for obstacles or conditions
        if(get_create_lbump() || get_create_rbump()) {
            emergency_stop = 1;
            printf("Emergency stop triggered!\n");
        }

        msleep(10);  // Check sensors every 10ms
    }
    return 0;
}
*/
