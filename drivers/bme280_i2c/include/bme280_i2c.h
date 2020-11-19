#ifndef __BME280_I2C_H__
#define __BME280_I2C_H__

#include "bme280.h"
#include "driver/i2c.h"

struct bme280_i2c_intf {
    i2c_port_t i2c_port;
    uint8_t dev_addr;
};

struct bme280_dev_i2c {
    struct bme280_dev dev;
    struct bme280_i2c_intf i2c_intf;
};

int8_t bme280_init_i2c(struct bme280_dev_i2c* dev_i2c, i2c_port_t i2c_port, uint8_t dev_addr);

#endif /* __BME280_I2C_H__ */
