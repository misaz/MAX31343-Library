#ifndef __MAX31343_PLATFORM_SPECIFIC_H
#define __MAX31343_PLATFORM_SPECIFIC_H

#include "MAX31343.h"
#include "stm32l5xx_hal.h"

#include <stdint.h>

#define MAX31343_I2C 					I2C1
#define MAX31343_I2C_CLK_EN				__HAL_RCC_I2C1_CLK_ENABLE

#define MAX31343_SDA_GPIO				GPIOB
#define MAX31343_SDA_GPIO_PIN			GPIO_PIN_9
#define MAX31343_SDA_GPIO_CLOCK_EN		__HAL_RCC_GPIOB_CLK_ENABLE
#define MAX31343_SDA_GPIO_AF			GPIO_AF4_I2C1

#define MAX31343_SCL_GPIO				GPIOB
#define MAX31343_SCL_GPIO_PIN			GPIO_PIN_8
#define MAX31343_SCL_GPIO_CLOCK_EN		__HAL_RCC_GPIOB_CLK_ENABLE
#define MAX31343_SCL_GPIO_AF			GPIO_AF4_I2C1

#define MAX31343_INT_GPIO				GPIOF
#define MAX31343_INT_GPIO_PIN			GPIO_PIN_15
#define MAX31343_INT_GPIO_CLOCK_EN		__HAL_RCC_GPIOF_CLK_ENABLE
#define MAX31343_INT_IRQn				EXTI15_IRQn
#define MAX31343_INT_PRIORITY			7

MAX31343_Status MAX31343_PlatformSpecific_Init();
MAX31343_Status MAX31343_PlatformSpecific_Deinit();
MAX31343_Status MAX31343_PlatformSpecific_ReadRegisters(uint8_t registerAddress, uint8_t* buffer, size_t bufferSize);
MAX31343_Status MAX31343_PlatformSpecific_WriteRegisters(uint8_t registerAddress, uint8_t* buffer, size_t bufferSize);
MAX31343_Status MAX31343_PlatformSpecific_ConfigureExternalInterrupt();

#endif
