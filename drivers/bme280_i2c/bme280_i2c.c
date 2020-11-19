#include "bme280_i2c.h"

#define WRITE_BIT     I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT      I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN  0x1              /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0              /*!< I2C master will not check ack from slave */
#define ACK_VAL       0x0              /*!< I2C ack value */
#define NACK_VAL      0x1              /*!< I2C nack value */

static void user_delay_us(uint32_t period, void* intf_ptr) {
    vTaskDelay(period / 1000 / portTICK_PERIOD_MS);
}

static int8_t user_i2c_read(uint8_t reg_addr, uint8_t* reg_data, uint32_t len, void* intf_ptr) {
    struct bme280_i2c_intf* i2c_intf = (struct bme280_i2c_intf*) intf_ptr;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (i2c_intf->dev_addr << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, reg_addr, ACK_CHECK_EN);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (i2c_intf->dev_addr << 1) | READ_BIT, ACK_CHECK_EN);
    if (len > 1) {
        i2c_master_read(cmd, reg_data, len - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, reg_data + len - 1, NACK_VAL);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_intf->i2c_port, cmd, 50 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    return ESP_OK == ret ? BME280_OK : -1;
}

static int8_t user_i2c_write(uint8_t reg_addr, const uint8_t* reg_data, uint32_t len, void* intf_ptr) {
    struct bme280_i2c_intf* i2c_intf = (struct bme280_i2c_intf*) intf_ptr;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (i2c_intf->dev_addr << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, reg_addr, ACK_CHECK_EN);
    i2c_master_write(cmd, reg_data, len, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_intf->i2c_port, cmd, 50 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    return ESP_OK == ret ? BME280_OK : -1;
}

int8_t bme280_init_i2c(struct bme280_dev_i2c* dev_i2c, i2c_port_t i2c_port, uint8_t dev_addr) {
    int8_t rslt = BME280_OK;
    dev_i2c->i2c_intf.i2c_port = i2c_port;
    dev_i2c->i2c_intf.dev_addr = dev_addr;

    dev_i2c->dev.intf_ptr = &dev_i2c->i2c_intf;
    dev_i2c->dev.intf = BME280_I2C_INTF;
    dev_i2c->dev.read = user_i2c_read;
    dev_i2c->dev.write = user_i2c_write;
    dev_i2c->dev.delay_us = user_delay_us;

    rslt = bme280_init(&dev_i2c->dev);

    return rslt;
}
