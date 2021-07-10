#include "MAX31343_PlatformSpecific.h"

static I2C_HandleTypeDef i2c;

MAX31343_Status MAX31343_PlatformSpecific_Init() {
	HAL_StatusTypeDef hStatus;

	i2c.Instance = MAX31343_I2C;
	i2c.Init.Timing = 0x00A03AC8;
	i2c.Init.OwnAddress1 = 0;
	i2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	i2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	i2c.Init.OwnAddress2 = 0;
	i2c.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	i2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	i2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	hStatus = HAL_I2C_Init(&i2c);
	if (hStatus) {
		return MAX31343_Status_I2CError;
	}

	hStatus = HAL_I2CEx_ConfigAnalogFilter(&i2c, I2C_ANALOGFILTER_ENABLE);
	if (hStatus) {
		return MAX31343_Status_I2CError;
	}

	hStatus = HAL_I2CEx_ConfigDigitalFilter(&i2c, 0);
	if (hStatus) {
		return MAX31343_Status_I2CError;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_PlatformSpecific_Deinit() {
	HAL_StatusTypeDef hStatus;

	hStatus = HAL_I2C_DeInit(&i2c);
	if (hStatus) {
		return MAX31343_Status_I2CError;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_PlatformSpecific_ConfigureExternalInterrupt() {
	GPIO_InitTypeDef gpio;
	gpio.Alternate = 0;
	gpio.Pin = MAX31343_INT_GPIO_PIN;
	gpio.Mode = GPIO_MODE_IT_FALLING;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;

	MAX31343_INT_GPIO_CLOCK_EN();
	HAL_GPIO_Init(MAX31343_INT_GPIO, &gpio);

	HAL_NVIC_SetPriority(MAX31343_INT_IRQn, MAX31343_INT_PRIORITY, 0);
	HAL_NVIC_EnableIRQ(MAX31343_INT_IRQn);

	// STM32 do not support level interrupt, so we must check if interrupt has not
	// occurred before we enabled this feature (for example in time when device was
	// power off)
	if (HAL_GPIO_ReadPin(MAX31343_INT_GPIO, MAX31343_INT_GPIO_PIN) == 0) {
		void (*interruptHandler)(void) =  (void (*)(void))NVIC_GetVector(MAX31343_INT_IRQn);
		(*interruptHandler)();
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_PlatformSpecific_ReadRegisters(uint8_t registerAddress, uint8_t *buffer, size_t bufferSize) {
	HAL_StatusTypeDef hStatus;

	hStatus = HAL_I2C_Mem_Read(&i2c, MAX31343_I2C_WRITE_ADDRESS, registerAddress, 1, buffer, bufferSize, 100);
	if (hStatus) {
		uint32_t errno = HAL_I2C_GetError(&i2c);

		if (errno & HAL_I2C_ERROR_AF) {
			return MAX31343_Status_I2CNack;
		} else if (errno & HAL_I2C_ERROR_TIMEOUT) {
			return MAX31343_Status_I2CTimeout;
		} else {
			return MAX31343_Status_I2CError;
		}
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_PlatformSpecific_WriteRegisters(uint8_t registerAddress, uint8_t *buffer, size_t bufferSize) {
	HAL_StatusTypeDef hStatus;

	hStatus = HAL_I2C_Mem_Write(&i2c, MAX31343_I2C_WRITE_ADDRESS, registerAddress, 1, buffer, bufferSize, 100);
	if (hStatus) {
		uint32_t errno = HAL_I2C_GetError(&i2c);

		if (errno & HAL_I2C_ERROR_AF) {
			return MAX31343_Status_I2CNack;
		} else if (errno & HAL_I2C_ERROR_TIMEOUT) {
			return MAX31343_Status_I2CTimeout;
		} else {
			return MAX31343_Status_I2CError;
		}
	}

	return MAX31343_Status_Ok;
}
