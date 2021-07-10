#include "MAX31343_PlatformSpecific.h"

#include <cyhal_i2c.h>
#include <cyhal_gpio.h>

static cyhal_i2c_t i2c;

__attribute__((weak)) void MAX31343_InterruptCallback(void *callback_arg, cyhal_gpio_event_t event) {

}

MAX31343_Status MAX31343_PlatformSpecific_Init() {
	cy_rslt_t status;

	status = cyhal_i2c_init(&i2c, MAX31343_PORT_SDA, MAX31343_PORT_SCL, NULL);
	if (status) {
		return MAX31343_Status_I2CError;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_PlatformSpecific_Deinit() {
	cyhal_i2c_free(&i2c);

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_PlatformSpecific_ConfigureExternalInterrupt() {
	cy_rslt_t cStatus;

	cStatus = cyhal_gpio_init(MAX31343_PORT_INT, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULL_NONE, 0);
	if (cStatus) {
		return MAX31343_Status_InvalidOperation;
	}

	cyhal_gpio_register_callback(MAX31343_PORT_INT, MAX31343_InterruptCallback, NULL);
	cyhal_gpio_enable_event(MAX31343_PORT_INT, CYHAL_GPIO_IRQ_FALL, MAX31343_INT_PRIORITY, 1);
	__enable_irq();

	// PSoC 6 do not support level interrupt, so we must check if interrupt has not
	// occurred before we enabled this feature (for example in time when device was
	// power off)
	if (cyhal_gpio_read(MAX31343_PORT_INT) == 0) {
		MAX31343_InterruptCallback(NULL, CYHAL_GPIO_IRQ_FALL);
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_PlatformSpecific_ReadRegisters(uint8_t registerAddress, uint8_t* buffer, size_t bufferSize) {
	cy_rslt_t status;

	status = cyhal_i2c_master_mem_read(&i2c, MAX31343_I2C_7BIT_ADDRESS, registerAddress, 1, buffer, bufferSize, 250);
	if (status == CY_SCB_I2C_MASTER_MANUAL_TIMEOUT) {
		return MAX31343_Status_I2CTimeout;
	} else if (	status == CY_SCB_I2C_MASTER_MANUAL_ADDR_NAK ||
				status == CY_SCB_I2C_MASTER_MANUAL_NAK) {
		return MAX31343_Status_I2CNack;
	} else if (status) {
		return MAX31343_Status_I2CError;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_PlatformSpecific_WriteRegisters(uint8_t registerAddress, uint8_t* buffer, size_t bufferSize) {
	cy_rslt_t status;

	status = cyhal_i2c_master_mem_write(&i2c, MAX31343_I2C_7BIT_ADDRESS, registerAddress, 1, buffer, bufferSize, 250);
	if (status == CY_SCB_I2C_MASTER_MANUAL_TIMEOUT) {
		return MAX31343_Status_I2CTimeout;
	} else if (	status == CY_SCB_I2C_MASTER_MANUAL_ADDR_NAK ||
				status == CY_SCB_I2C_MASTER_MANUAL_NAK) {
		return MAX31343_Status_I2CNack;
	} else if (status) {
		return MAX31343_Status_I2CError;
	}

	return MAX31343_Status_Ok;
}
