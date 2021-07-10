#include "MAX31343_PlatformSpecific.h"

MAX31343_Status MAX31343_PlatformSpecific_Init() {
	// this function must initialize I2C bus driver. It should not generate any 
	// transaction to device. It is recommended to generate 9 clock cycles when SDA 
	// line is asserted as 0 and then generate stop sequence.

	return MAX31343_Status_NotImplemented;
}

MAX31343_Status MAX31343_PlatformSpecific_Deinit() {
	// this function must deinitialize I2C bus driver. It should not generate any 
	// transaction to device.

	return MAX31343_Status_NotImplemented;
}

MAX31343_Status MAX31343_PlatformSpecific_ConfigureExternalInterrupt() {
	// this function should initialize hardware for 
	return MAX31343_Status_NotImplemented;
}

MAX31343_Status MAX31343_PlatformSpecific_ReadRegisters(uint8_t registerAddress, uint8_t* buffer, size_t bufferSize) {
	// this function must generate following on I2C bus:
	//
	// 1) generate STRAT condition.
	// 2) Transmit MAX31343_I2C_WRITE_ADDRESS. If you are using platform (library) which 
	//    requires non-shifted 7bit address use MAX31343_I2C_7BIT_ADDRESS constant and 
	//    write direction.
	// 3) check that device ACKed transaction. Otherwise generate STOP sequence and 
	//    return MAX31343_Status_I2CNack.
	// 4) transmit registerAddress byte.
	// 5) check that device ACKed transaction. Otherwise generate STOP sequence and
	//    return MAX31343_Status_I2CNack.
	// 6) generate REPEATED START sequence
	// 7) Transmit MAX31343_I2C_READ_ADDRESS. If you are using platform (library) which 
	//    requires non-shifted 7bit address use MAX31343_I2C_7BIT_ADDRESS constant and 
	//    read direction.
	// 8) read buffer of size bufferSize. After receiving byte ACK the transaction. 
	//    Generate NACK instead of ACK after receiving last byte.
	// 7) generate STOP sequence and return MAX31343_Status_Ok.
	//
	// In all steps check for errors like arbitration lost error. Use return code 
	// MAX31343_Status_I2CNack for errors related to NACKing and MAX31343_Status_I2CError 
	// for other I2C errors. 
	//
	// Before returning any status code remember to generate STOP sequence.
	//
	// Prevent using loops which may end up in inifinete loop in case of unpredictable 
	// behaviour of I2C peripheral. Try implement platform specific timeout detection
	// and return MAX31343_Status_I2CTimeout when timeout occur.

	return MAX31343_Status_NotImplemented;
}

MAX31343_Status MAX31343_PlatformSpecific_WriteRegisters(uint8_t registerAddress, uint8_t* buffer, size_t bufferSize) {
	// this function must generate following on I2C bus:
	//
	// 1) generate STRAT condition.
	// 2) Transmit MAX31343_I2C_WRITE_ADDRESS. If you are using platform (library) which 
	//    requires non-shifted 7bit address use MAX31343_I2C_7BIT_ADDRESS constant and 
	//    write direction.
	// 3) check that device ACKed transaction. Otherwise generate STOP sequence and 
	//    return MAX31343_Status_I2CNack.
	// 4) transmit registerAddress byte.
	// 5) check that device ACKed transaction. Otherwise generate STOP sequence and
	//    return MAX31343_Status_I2CNack.
	// 6) transmit buffer of size bufferSize. After transmiting every byte check that 
	//    device ACKed transaction. Otherwise generate STOP sequence and return
	//    MAX31343_Status_I2CNack.
	// 7) generate STOP sequence and return MAX31343_Status_Ok.
	//
	// In all steps check for errors like arbitration lost error. Use return code 
	// MAX31343_Status_I2CNack for errors related to NACKing and MAX31343_Status_I2CError 
	// for other I2C errors. 
	//
	// Before returning any status code remember to generate STOP sequence.
	//
	// Prevent using loops which may end up in inifinete loop in case of unpredictable 
	// behaviour of I2C peripheral. Try implement platform specific timeout detection
	// and return MAX31343_Status_I2CTimeout when timeout occur.

	return MAX31343_Status_NotImplemented;
}
