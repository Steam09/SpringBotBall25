#ifndef VARIABLES_H
#define VARIABLES_H

typedef struct {
    int frontLeft;
    int frontRight;
    int backLeft;
    int backRight;
} WheelPorts;

// Define the wheel port assignments
static const WheelPorts wheelPorts = {
    0,  // Port 0 - frontLeft
    3,  // Port 3 - frontRight
    1,  // Port 1 - backLeft
    2   // Port 2 - backRight
};

typedef struct {
    int frontLeft;
    int frontRight;
    int backLeft;
    int backRight;
} ServoPorts;

static const ServoPorts servoPorts = {
    0,  // Port 1 - frontLeft
    1,  // Port 2 - frontRight
    2,  // Port 3 - backLeft
    3   // Port 4 - backRight
};

typedef struct {
    int frontLeft;
    int frontRight;
    int backLeft;
    int backRight;
} AnalogPorts;

static const AnalogPorts analogPorts = {
    0,  // Port 1 - frontLeft
    1,  // Port 2 - frontRight
    2,  // Port 3 - backLeft
    3   // Port 4 - backRight
};

#endif // VARIABLES_H
