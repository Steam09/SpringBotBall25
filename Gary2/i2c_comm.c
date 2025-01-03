#include "../include/i2c_comm.h"
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <kipr/wombat.h>

static int i2c_fd;

void i2c_init(void) {
    char *filename = "/dev/i2c-1";
    if ((i2c_fd = open(filename, O_RDWR)) < 0) {
        printf("Failed to open I2C bus\n");
        return;
    }
}

unsigned char i2c_read_byte(unsigned char addr, unsigned char reg) {
    unsigned char buf[1];
    if (ioctl(i2c_fd, I2C_SLAVE, addr) < 0) {
        printf("Failed to acquire bus access\n");
        return 0;
    }

    write(i2c_fd, &reg, 1);
    read(i2c_fd, buf, 1);
    return buf[0];
}

void i2c_write_byte(unsigned char addr, unsigned char reg, unsigned char data) {
    unsigned char buf[2] = {reg, data};
    if (ioctl(i2c_fd, I2C_SLAVE, addr) < 0) {
        printf("Failed to acquire bus access\n");
        return;
    }

    write(i2c_fd, buf, 2);
}

void i2c_read_bytes(unsigned char addr, unsigned char reg, unsigned char *data, unsigned char length) {
    if (ioctl(i2c_fd, I2C_SLAVE, addr) < 0) {
        printf("Failed to acquire bus access\n");
        return;
    }

    write(i2c_fd, &reg, 1);
    read(i2c_fd, data, length);
}
