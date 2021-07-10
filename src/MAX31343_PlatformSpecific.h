#ifndef __MAX31343_PLATFORM_SPECIFIC_H
#define __MAX31343_PLATFORM_SPECIFIC_H

#include "MAX31343.h"

#include <stdint.h>

MAX31343_Status MAX31343_PlatformSpecific_Init();
MAX31343_Status MAX31343_PlatformSpecific_Deinit();
MAX31343_Status MAX31343_PlatformSpecific_ReadRegisters(uint8_t registerAddress, uint8_t* buffer, size_t bufferSize);
MAX31343_Status MAX31343_PlatformSpecific_WriteRegisters(uint8_t registerAddress, uint8_t* buffer, size_t bufferSize);
MAX31343_Status MAX31343_PlatformSpecific_ConfigureExternalInterrupt();

#endif