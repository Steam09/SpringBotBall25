#ifndef I2C_COMM_H
#define I2C_COMM_H

#include <stdint.h>
#include <stdio.h>

void i2c_init(void);
unsigned char i2c_read_byte(unsigned char addr, unsigned char reg);
void i2c_write_byte(unsigned char addr, unsigned char reg, unsigned char data);
void i2c_read_bytes(unsigned char addr, unsigned char reg, unsigned char *data, unsigned char length);

#endif
