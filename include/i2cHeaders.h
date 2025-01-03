#ifndef I2C_FUNCTIONS_H
#define I2C_FUNCTIONS_H

extern int i2c_read8(int address, int reg);
extern int i2c_write8(int address, int reg, int value);
extern void i2c_init();

#endif
