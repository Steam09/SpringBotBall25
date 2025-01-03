#include "../include/preRun.h"
#include <kipr/button/button.h>
#include <kipr/wombat.h>

void checks() {
    printf("Pre-Run Checks\n");
    printf("Battery percentage: %f\n", power_level());
    printf("Ready?\n");
    while (right_button() == 0) {
        msleep(1);
    }
    return;
}
