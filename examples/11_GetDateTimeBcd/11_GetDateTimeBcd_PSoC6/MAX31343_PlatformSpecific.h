#ifndef __MAX31343_PLATFORM_SPECIFIC_H
#define __MAX31343_PLATFORM_SPECIFIC_H

#include "MAX31343.h"

#include <stdint.h>

#define MAX31343_PORT_SDA		P6_1
#define MAX31343_PORT_SCL		P6_0

#define MAX31343_PORT_INT		P5_2
#define MAX31343_INT_PRIORITY	7

MAX31343_Status MAX31343_PlatformSpecific_Init();
MAX31343_Status MAX31343_PlatformSpecific_Deinit();
MAX31343_Status MAX31343_PlatformSpecific_ReadRegisters(uint8_t registerAddress, uint8_t* buffer, size_t bufferSize);
MAX31343_Status MAX31343_PlatformSpecific_WriteRegisters(uint8_t registerAddress, uint8_t* buffer, size_t bufferSize);
MAX31343_Status MAX31343_PlatformSpecific_ConfigureExternalInterrupt();

#endif
