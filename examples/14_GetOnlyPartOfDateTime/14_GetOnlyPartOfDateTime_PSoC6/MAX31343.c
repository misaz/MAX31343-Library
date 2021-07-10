#include "MAX31343.h"
#include "MAX31343_PlatformSpecific.h"

#define MAX31343_VALUE_TO_BCD(value) (((value) % 10) | (((value) / 10) << 4))
#define MAX31343_BCD_TO_VALUE(bcd) (((bcd) & 0xF) + ((((bcd) & 0xF0) >> 4) * 10))

#define MAX31343_EXPAND_MACRO(content) content

#define MAX31343_FIELD_OFFSET_IMPL(reg,nonShiftedBitMask,offset) (offset)
#define MAX31343_FIELD_OFFSET(...) MAX31343_EXPAND_MACRO(MAX31343_FIELD_OFFSET_IMPL(__VA_ARGS__))

#define MAX31343_FIELD_MASK_IMPL(reg,nonShiftedBitMask,offset) ((nonShiftedBitMask) << (offset))
#define MAX31343_FIELD_MASK(...) MAX31343_EXPAND_MACRO(MAX31343_FIELD_MASK_IMPL(__VA_ARGS__))

#define MAX31343_BCD_IS_FIRST_DIGIT_INVALID(bcdNum) (((bcdNum) & 0xF) > 9)
#define MAX31343_BCD_IS_SECOND_DIGIT_INVALID(bcdNum) (((bcdNum) & 0xF0) > 0x90)

static MAX31343_Status MAX31343_ReadRegisters(uint8_t registerAddress, uint8_t* buffer, size_t bufferSize) {
	return MAX31343_PlatformSpecific_ReadRegisters(registerAddress, buffer, bufferSize);
}

static MAX31343_Status MAX31343_ReadSingleRegister(uint8_t registerAddress, uint8_t* value) {
	return MAX31343_PlatformSpecific_ReadRegisters(registerAddress, value, 1);
}

static MAX31343_Status MAX31343_WriteRegisters(uint8_t registerAddress, uint8_t* buffer, size_t bufferSize) {
	return MAX31343_PlatformSpecific_WriteRegisters(registerAddress, buffer, bufferSize);
}

static MAX31343_Status MAX31343_WriteSingleRegister(uint8_t registerAddress, uint8_t value) {
	return MAX31343_PlatformSpecific_WriteRegisters(registerAddress, &value, 1);
}

static MAX31343_Status MAX31343_WriteField(uint8_t registerAddress, uint8_t fieldNonShiftedMask, uint8_t fieldOffset, uint8_t newValue) {
	MAX31343_Status status;
	uint8_t originalRegisterValue;
	uint8_t newRegisterValue;

	status = MAX31343_ReadSingleRegister(registerAddress, &originalRegisterValue);
	if (status) {
		return status;
	}

	newRegisterValue = (originalRegisterValue & ~(fieldNonShiftedMask << fieldOffset)) | ((newValue & fieldNonShiftedMask) << fieldOffset);

	if (newRegisterValue != originalRegisterValue) {
		status = MAX31343_WriteSingleRegister(registerAddress, newValue);
		if (status) {
			return status;
		}
	}

	return MAX31343_Status_Ok;
}

static MAX31343_Status MAX31343_WriteBoolField(uint8_t registerAddress, uint8_t fieldNonShiftedMask, uint8_t fieldOffset, int newValue) {
	MAX31343_Status status;
	uint8_t originalRegisterValue;
	uint8_t newRegisterValue;

	status = MAX31343_ReadSingleRegister(registerAddress, &originalRegisterValue);
	if (status) {
		return status;
	}

	newRegisterValue = (originalRegisterValue & ~(1 << fieldOffset)) | (!!newValue << fieldOffset);

	if (newRegisterValue != originalRegisterValue) {
		status = MAX31343_WriteSingleRegister(registerAddress, newRegisterValue);
		if (status) {
			return status;
		}
	}

	return MAX31343_Status_Ok;
}

static MAX31343_Status MAX31343_ReadField(uint8_t registerAddress, uint8_t fieldNonShiftedMask, uint8_t fieldOffset, uint8_t* value) {
	MAX31343_Status status;
	uint8_t registerValue;

	status = MAX31343_ReadSingleRegister(registerAddress, &registerValue);
	if (status) {
		return status;
	}

	*value = (registerValue >> fieldOffset) & fieldNonShiftedMask;
	return MAX31343_Status_Ok;
}

static MAX31343_Status MAX31343_ReadBoolField(uint8_t registerAddress, uint8_t fieldNonShiftedMask, uint8_t fieldOffset, int* value) {
	MAX31343_Status status;
	uint8_t registerValue;

	status = MAX31343_ReadSingleRegister(registerAddress, &registerValue);
	if (status) {
		return status;
	}

	*value = !!(registerValue & (1 << fieldOffset));
	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_ValidateTime(MAX31343_DateTime* datetime) {
	if (datetime->isBcdFormatUsed) {
		if (MAX31343_BCD_IS_FIRST_DIGIT_INVALID(datetime->seconds) ||
			MAX31343_BCD_IS_SECOND_DIGIT_INVALID(datetime->seconds) ||
			datetime->seconds > 0x59 ||
			MAX31343_BCD_IS_FIRST_DIGIT_INVALID(datetime->minutes) ||
			MAX31343_BCD_IS_SECOND_DIGIT_INVALID(datetime->minutes) ||
			datetime->minutes > 0x59 ||
			MAX31343_BCD_IS_FIRST_DIGIT_INVALID(datetime->hours) ||
			MAX31343_BCD_IS_SECOND_DIGIT_INVALID(datetime->hours) ||
			datetime->hours > 0x23) {

			return MAX31343_Status_BadArg;
		}
	} else {
		if (datetime->seconds > 59 ||
			datetime->minutes > 59 ||
			datetime->hours > 23) {

			return MAX31343_Status_BadArg;
		}
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_ValidateDate(MAX31343_DateTime* datetime) {
	if (datetime->isBcdFormatUsed) {
		if (datetime->dayOfWeek == 0 ||
			datetime->dayOfWeek > 0x7 ||
			MAX31343_BCD_IS_FIRST_DIGIT_INVALID(datetime->date) ||
			MAX31343_BCD_IS_SECOND_DIGIT_INVALID(datetime->date) ||
			datetime->date > 0x31 ||
			MAX31343_BCD_IS_FIRST_DIGIT_INVALID(datetime->month) ||
			MAX31343_BCD_IS_SECOND_DIGIT_INVALID(datetime->month) ||
			datetime->month == 0 ||
			datetime->month > 0x12 ||
			MAX31343_BCD_IS_FIRST_DIGIT_INVALID(datetime->year) ||
			MAX31343_BCD_IS_SECOND_DIGIT_INVALID(datetime->year) ||
			datetime->century > 0x1) {

			return MAX31343_Status_BadArg;
		}
	} else {
		if (datetime->dayOfWeek == 0 ||
			datetime->dayOfWeek > 7 ||
			datetime->date > 31 ||
			datetime->month == 0 ||
			datetime->month > 12 ||
			datetime->year > 99 ||
			datetime->century > 1) {
			return MAX31343_Status_BadArg;
		}
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_ValidateDateTime(MAX31343_DateTime* datetime) {
	MAX31343_Status status;

	status = MAX31343_ValidateTime(datetime);
	if (status) {
		return status;
	}

	status = MAX31343_ValidateDate(datetime);
	if (status) {
		return status;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Init() {
	MAX31343_Status status;

	status = MAX31343_PlatformSpecific_Init();
	if (status) {
		return status;
	}

	// check that at least one register is readable (this is mainly used for detecting 
	// presence of device and reporting error when device do not ACK request)
	uint8_t dummy;
	return MAX31343_ReadSingleRegister(MAX31343_REGISTER_SECONDS, &dummy);
}

MAX31343_Status MAX31343_Deinit() {
	return MAX31343_PlatformSpecific_Deinit();
}

MAX31343_Status MAX31343_InitInterruptsSupport() {
	return MAX31343_PlatformSpecific_ConfigureExternalInterrupt();
}

MAX31343_Status MAX31343_Reset() {
	MAX31343_Status status;

	status = MAX31343_WriteField(MAX31343_FIELD_SWRST, 1);
	if (status) {
		return status;
	}

	status = MAX31343_WriteField(MAX31343_FIELD_SWRST, 0);
	if (status) {
		return status;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_ConfigureInterrupts(MAX31343_Interrupt enabledInterrupts) {
	// check for unallowed bit
	if (enabledInterrupts & 0x90) {
		return MAX31343_Status_BadArg;
	}

	return MAX31343_WriteSingleRegister(MAX31343_REGISTER_INT_EN, enabledInterrupts);
}

MAX31343_Status MAX31343_EnableInterrupts(MAX31343_Interrupt enableInterrupts) {
	MAX31343_Status status;
	uint8_t currentlyEnabledInterrupts;
	uint8_t newlyEnabledInterrupts;

	// check for unallowed bit
	if (enableInterrupts & 0x90) {
		return MAX31343_Status_BadArg;
	}

	status = MAX31343_ReadSingleRegister(MAX31343_REGISTER_INT_EN, &currentlyEnabledInterrupts);
	if (status) {
		return status;
	}

	newlyEnabledInterrupts = currentlyEnabledInterrupts | enableInterrupts;

	return MAX31343_WriteSingleRegister(MAX31343_REGISTER_INT_EN, newlyEnabledInterrupts);
}

MAX31343_Status MAX31343_DisableInterrupts(MAX31343_Interrupt disableInterrupts) {
	MAX31343_Status status;
	uint8_t currentlyEnabledInterrupts;
	uint8_t newlyEnabledInterrupts;

	// check for unallowed bit
	if (disableInterrupts & 0x90) {
		return MAX31343_Status_BadArg;
	}

	status = MAX31343_ReadSingleRegister(MAX31343_REGISTER_INT_EN, &currentlyEnabledInterrupts);
	if (status) {
		return status;
	}

	newlyEnabledInterrupts = currentlyEnabledInterrupts & ~disableInterrupts;

	return MAX31343_WriteSingleRegister(MAX31343_REGISTER_INT_EN, newlyEnabledInterrupts);
}

MAX31343_Status MAX31343_GetPendingInterrups(MAX31343_Interrupt* flags) {
	MAX31343_Status status;

	status = MAX31343_ReadField(MAX31343_FIELD_INTFLAGS, flags);
	if (status) {
		return status;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_SetDateTime(MAX31343_DateTime* datetime) {
	MAX31343_Status status;
	uint8_t registers[7];

	if (datetime == NULL) {
		return MAX31343_Status_BadArg;
	}

	status = MAX31343_ValidateDateTime(datetime);
	if (status) {
		return status;
	}

	if (datetime->isBcdFormatUsed) {
		registers[0] = datetime->seconds;
		registers[1] = datetime->minutes;
		registers[2] = datetime->hours;
		registers[3] = datetime->dayOfWeek;
		registers[4] = datetime->date;
		registers[5] = datetime->month | (datetime->century << 7);
		registers[6] = datetime->year;
	} else {
		registers[0] = MAX31343_VALUE_TO_BCD(datetime->seconds);
		registers[1] = MAX31343_VALUE_TO_BCD(datetime->minutes);
		registers[2] = MAX31343_VALUE_TO_BCD(datetime->hours);
		registers[3] = MAX31343_VALUE_TO_BCD(datetime->dayOfWeek);
		registers[4] = MAX31343_VALUE_TO_BCD(datetime->date);
		registers[5] = MAX31343_VALUE_TO_BCD(datetime->month) | (datetime->century << 7);
		registers[6] = MAX31343_VALUE_TO_BCD(datetime->year);
	}

	return MAX31343_WriteRegisters(MAX31343_REGISTER_SECONDS, registers, sizeof(registers));
}

MAX31343_Status MAX31343_SetDate(MAX31343_DateTime* date) {
	MAX31343_Status status;
	uint8_t registers[4];

	if (date == NULL) {
		return MAX31343_Status_BadArg;
	}

	status = MAX31343_ValidateDate(date);
	if (status) {
		return status;
	}

	if (date->isBcdFormatUsed) {
		registers[0] = date->dayOfWeek;
		registers[1] = date->date;
		registers[2] = date->month | (date->century << 7);
		registers[3] = date->year;
	} else {
		registers[0] = MAX31343_VALUE_TO_BCD(date->dayOfWeek);
		registers[1] = MAX31343_VALUE_TO_BCD(date->date);
		registers[2] = MAX31343_VALUE_TO_BCD(date->month) | (date->century << 7);
		registers[3] = MAX31343_VALUE_TO_BCD(date->year);
	}

	return MAX31343_WriteRegisters(MAX31343_REGISTER_DAY, registers, sizeof(registers));
}

MAX31343_Status MAX31343_SetTime(MAX31343_DateTime* time) {
	MAX31343_Status status;
	uint8_t registers[3];

	if (time == NULL) {
		return MAX31343_Status_BadArg;
	}

	status = MAX31343_ValidateTime(time);
	if (status) {
		return status;
	}

	if (time->isBcdFormatUsed) {
		registers[0] = time->seconds;
		registers[1] = time->minutes;
		registers[2] = time->hours;
	} else {
		registers[0] = MAX31343_VALUE_TO_BCD(time->seconds);
		registers[1] = MAX31343_VALUE_TO_BCD(time->minutes);
		registers[2] = MAX31343_VALUE_TO_BCD(time->hours);
	}

	return MAX31343_WriteRegisters(MAX31343_REGISTER_SECONDS, registers, sizeof(registers));
}

MAX31343_Status MAX31343_GetDateTime(MAX31343_DateTime* datetime, int getDateTimeInBcdFormat) {
	MAX31343_Status status;
	uint8_t registers[7];

	if (datetime == NULL) {
		return MAX31343_Status_BadArg;
	}

	status = MAX31343_ReadRegisters(MAX31343_REGISTER_SECONDS, registers, sizeof(registers));
	if (status) {
		return status;
	}

	datetime->isBcdFormatUsed = getDateTimeInBcdFormat;
	if (getDateTimeInBcdFormat) {
		datetime->seconds = registers[0];
		datetime->minutes = registers[1];
		datetime->hours = registers[2];
		datetime->dayOfWeek = registers[3];
		datetime->date = registers[4];
		datetime->month = registers[5] & 0x1F;
		datetime->century = registers[5] >> 7;
		datetime->year = registers[6];
	} else {
		datetime->seconds = MAX31343_BCD_TO_VALUE(registers[0]);
		datetime->minutes = MAX31343_BCD_TO_VALUE(registers[1]);
		datetime->hours = MAX31343_BCD_TO_VALUE(registers[2]);
		datetime->dayOfWeek = MAX31343_BCD_TO_VALUE(registers[3]);
		datetime->date = MAX31343_BCD_TO_VALUE(registers[4]);
		datetime->month = MAX31343_BCD_TO_VALUE(registers[5] & 0x1F);
		datetime->century = registers[5] >> 7;
		datetime->year = MAX31343_BCD_TO_VALUE(registers[6]);
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_GetDate(MAX31343_DateTime* date, int getDateInBcdFormat) {
	MAX31343_Status status;
	uint8_t registers[4];

	if (date == NULL) {
		return MAX31343_Status_BadArg;
	}

	status = MAX31343_ReadRegisters(MAX31343_REGISTER_DAY, registers, sizeof(registers));
	if (status) {
		return status;
	}

	date->isBcdFormatUsed = getDateInBcdFormat;
	if (getDateInBcdFormat) {
		date->dayOfWeek = registers[0];
		date->date = registers[1];
		date->month = registers[2] & MAX31343_FIELD_MASK(MAX31343_FIELD_MONTH);
		date->century = registers[2] >> MAX31343_FIELD_OFFSET(MAX31343_FIELD_CENTURY);
		date->year = registers[3];
	} else {
		date->dayOfWeek = MAX31343_BCD_TO_VALUE(registers[0]);
		date->date = MAX31343_BCD_TO_VALUE(registers[1]);
		date->month = MAX31343_BCD_TO_VALUE(registers[2] & 0x1F);
		date->century = registers[2] >> MAX31343_FIELD_OFFSET(MAX31343_FIELD_CENTURY);
		date->year = MAX31343_BCD_TO_VALUE(registers[3]);
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_GetTime(MAX31343_DateTime* time, int getTimeInBcdFormat) {
	MAX31343_Status status;
	uint8_t registers[3];

	if (time == NULL) {
		return MAX31343_Status_BadArg;
	}

	status = MAX31343_ReadRegisters(MAX31343_REGISTER_SECONDS, registers, sizeof(registers));
	if (status) {
		return status;
	}

	time->isBcdFormatUsed = getTimeInBcdFormat;
	if (getTimeInBcdFormat) {
		time->seconds = registers[0];
		time->minutes = registers[1];
		time->hours = registers[2];
	} else {
		time->seconds = MAX31343_BCD_TO_VALUE(registers[0]);
		time->minutes = MAX31343_BCD_TO_VALUE(registers[1]);
		time->hours = MAX31343_BCD_TO_VALUE(registers[2]);
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_DataRetention_Enable() {
	return MAX31343_WriteBoolField(MAX31343_FIELD_DATA_RET, 1);
}

MAX31343_Status MAX31343_DataRetention_Disable() {
	return MAX31343_WriteBoolField(MAX31343_FIELD_DATA_RET, 0);
}

MAX31343_Status MAX31343_DataRetention_IsEnabled(int* isDataRetentionEnabled) {
	return MAX31343_ReadBoolField(MAX31343_FIELD_DATA_RET, isDataRetentionEnabled);
}

MAX31343_Status MAX31343_Oscilator_Enable() {
	return MAX31343_WriteBoolField(MAX31343_FIELD_ENOSC, 1);
}

MAX31343_Status MAX31343_Oscilator_Disable() {
	return MAX31343_WriteBoolField(MAX31343_FIELD_ENOSC, 0);
}

MAX31343_Status MAX31343_Oscilator_IsEnabled(int* isOscilatorEnabled) {
	return MAX31343_ReadBoolField(MAX31343_FIELD_ENOSC, isOscilatorEnabled);
}

MAX31343_Status MAX31343_I2CTimeout_Enable() {
	return MAX31343_WriteBoolField(MAX31343_FIELD_I2C_TIMEOUT, 1);
}

MAX31343_Status MAX31343_I2CTimeout_Disable() {
	return MAX31343_WriteBoolField(MAX31343_FIELD_I2C_TIMEOUT, 0);
}

MAX31343_Status MAX31343_I2CTimeout_IsEnabled(int* isI2CTimeoutEnabled) {
	return MAX31343_ReadBoolField(MAX31343_FIELD_I2C_TIMEOUT, isI2CTimeoutEnabled);
}

MAX31343_Status MAX31343_OutputClocks_SetSqwFrequency(MAX31343_SqwFrequency sqwFrequency) {
	if (sqwFrequency < 0 || sqwFrequency > 7) {
		return MAX31343_Status_BadArg;
	}

	return MAX31343_WriteField(MAX31343_FIELD_SQW_HZ, sqwFrequency);
}

MAX31343_Status MAX31343_OutputClocks_GetSqwFrequency(MAX31343_SqwFrequency* sqwFrequency) {
	MAX31343_Status status;
	uint8_t fieldValue;

	status = MAX31343_ReadField(MAX31343_FIELD_SQW_HZ, &fieldValue);
	if (status) {
		return status;
	}

	// there are aliases for 32Hz which we map to one enum constant
	if (fieldValue > MAX31343_SqwFrequency_32Hz) {
		fieldValue = MAX31343_SqwFrequency_32Hz;
	}

	*sqwFrequency = fieldValue;
	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_OutputClocks_SetClkoFrequency(MAX31343_ClkoFrequency clkoFrequency) {
	if (clkoFrequency < 0 || clkoFrequency > 15) {
		return MAX31343_Status_BadArg;
	}

	return MAX31343_WriteField(MAX31343_FIELD_CLKO_HZ, clkoFrequency);
}

MAX31343_Status MAX31343_OutputClocks_GetClkoFrequency(MAX31343_ClkoFrequency* clkoFrequency) {
	MAX31343_Status status;
	uint8_t fieldValue;

	status = MAX31343_ReadField(MAX31343_FIELD_CLKO_HZ, &fieldValue);
	if (status) {
		return status;
	}

	// there are aliases for 32kHz which we map to one enum constant
	if (fieldValue > MAX31343_ClkoFrequency_32kHz) {
		fieldValue = MAX31343_ClkoFrequency_32kHz;
	}

	*clkoFrequency = fieldValue;
	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_OutputClocks_EnableClko() {
	return MAX31343_WriteBoolField(MAX31343_FIELD_ENCLKO, 1);
}

MAX31343_Status MAX31343_OutputClocks_DisableClko() {
	return MAX31343_WriteBoolField(MAX31343_FIELD_ENCLKO, 0);
}

MAX31343_Status MAX31343_OutputClocks_IsClkoEnabled(int* isClkoEnabled) {
	return MAX31343_ReadBoolField(MAX31343_FIELD_ENCLKO, isClkoEnabled);
}

MAX31343_Status MAX31343_Power_ConfigureAutomaticPowerSupplySelection(MAX31343_PowerFailTreshold powerFailTreshold) {
	// value 0 is also prohibited
	if (powerFailTreshold < 1 || powerFailTreshold > 3) {
		return MAX31343_Status_BadArg;
	}

	uint8_t value = powerFailTreshold << 4;

	return MAX31343_WriteSingleRegister(MAX31343_REGISTER_PWR_MGMT, value);
}

MAX31343_Status MAX31343_Power_ConfigureManualPowerSupplySelection(int enableBackupPowerSupply) {
	uint8_t value = MAX31343_FIELD_MASK(MAX31343_FIELD_D_MAN_SEL) | (!!enableBackupPowerSupply << MAX31343_FIELD_OFFSET(MAX31343_FIELD_D_VBACK_SEL));

	return MAX31343_WriteSingleRegister(MAX31343_REGISTER_PWR_MGMT, value);
}

MAX31343_Status MAX31343_Power_GetPowerSupplySelectionMode(MAX31343_PowerSupplySelectionMode* powerSupplySelectionMode) {
	MAX31343_Status status;
	uint8_t value;

	status = MAX31343_ReadField(MAX31343_FIELD_D_MAN_SEL, &value);
	if (status) {
		return status;
	}

	*powerSupplySelectionMode = value;
	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Power_GetPowerFailTreshold(MAX31343_PowerFailTreshold* powerFailTreshold) {
	MAX31343_Status status;
	uint8_t registerValue;

	status = MAX31343_ReadSingleRegister(MAX31343_REGISTER_PWR_MGMT, &registerValue);
	if (status) {
		return status;
	}

	// calling this function makes only sense in automatic power selection mode
	if (registerValue & MAX31343_FIELD_MASK(MAX31343_FIELD_D_MAN_SEL)) {
		return MAX31343_Status_InvalidOperation;
	}

	*powerFailTreshold = (registerValue & MAX31343_FIELD_MASK(MAX31343_FIELD_PFVT)) >> MAX31343_FIELD_OFFSET(MAX31343_FIELD_PFVT);
	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Power_IsBackupPowerSupplyEnabled(int* isBackupPowerSupplyEnabled) {
	MAX31343_Status status;
	uint8_t registerValue;

	status = MAX31343_ReadSingleRegister(MAX31343_REGISTER_PWR_MGMT, &registerValue);
	if (status) {
		return status;
	}

	// calling this function makes only sense in manual power selection mode
	if ((registerValue & MAX31343_FIELD_MASK(MAX31343_FIELD_D_MAN_SEL)) == 0) {
		return MAX31343_Status_InvalidOperation;
	}


	*isBackupPowerSupplyEnabled = !!(registerValue & MAX31343_FIELD_MASK(MAX31343_FIELD_D_VBACK_SEL));
	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Power_GetCurrentPowerSupplyUsed(MAX31343_PowerSupplyType* currentPowerSupplyUsed) {
	MAX31343_Status status;
	uint8_t value;

	status = MAX31343_ReadField(MAX31343_FIELD_PSDECT, &value);
	if (status) {
		return status;
	}

	*currentPowerSupplyUsed = value;
	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_TricleCharger_SetMode(MAX31343_TrickleChargerMode trickleChargerMode) {
	if (trickleChargerMode < 0 || trickleChargerMode > 15) {
		return MAX31343_Status_BadArg;
	}

	return MAX31343_WriteField(MAX31343_FIELD_TCHE, trickleChargerMode);
}

MAX31343_Status MAX31343_TricleCharger_Enable() {
	return MAX31343_WriteField(MAX31343_FIELD_TCHE, 0x5);
}

MAX31343_Status MAX31343_TricleCharger_Disable() {
	return MAX31343_WriteField(MAX31343_FIELD_TCHE, 0x0);
}

MAX31343_Status MAX31343_TricleCharger_IsEnabled(int* isTrickleChargerEnabled) {
	MAX31343_Status status;
	uint8_t value;

	status = MAX31343_ReadField(MAX31343_FIELD_TCHE, &value);
	if (status) {
		return status;
	}

	*isTrickleChargerEnabled = value == 0x5;
	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_TricleCharger_SetModeAndEnable(MAX31343_TrickleChargerMode trickleChargerMode) {
	if (trickleChargerMode < 0 || trickleChargerMode > 15) {
		return MAX31343_Status_BadArg;
	}

	uint8_t value = 0x50 | trickleChargerMode;
	return MAX31343_WriteSingleRegister(MAX31343_REGISTER_TRICKLE_REG, value);
}

MAX31343_Status MAX31343_TricleCharger_GetMode(MAX31343_TrickleChargerMode* trickleChargerMode) {
	MAX31343_Status status;
	uint8_t value;

	status = MAX31343_ReadField(MAX31343_FIELD_D_TRICKLE, &value);
	if (status) {
		return status;
	}

	// handle "aliases"
	if (value == 0x1) {
		value = MAX31343_TrickleChargerMode_3k_without_diode;
	} else if (value == 0x5) {
		value = MAX31343_TrickleChargerMode_3k_with_diode;
	} else if (value > 0x8) {
		value = MAX31343_TrickleChargerMode_no_connection;
	}

	*trickleChargerMode = value;
	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Timer_Configure(MAX31343_Timer_RepeatMode repeatMode, MAX31343_Timer_Frequency frequency, uint8_t autoReloadValue, int startImmediately) {
	MAX31343_Status status;

	if (repeatMode < 0 || repeatMode > 1 || frequency < 0 || frequency > 3) {
		return MAX31343_Status_BadArg;
	}

	startImmediately = !!startImmediately;

	// stop timer
	status = MAX31343_WriteSingleRegister(MAX31343_REGISTER_TIMER_CONFIG, 0);
	if (status) {
		return status;
	}

	// configure auto reload (initial) value
	status = MAX31343_WriteSingleRegister(MAX31343_REGISTER_TIMER_INIT, autoReloadValue);
	if (status) {
		return status;
	}

	uint8_t configurationRegisterValue =
		(repeatMode << MAX31343_FIELD_OFFSET(MAX31343_FIELD_TRPT)) |
		(frequency << MAX31343_FIELD_OFFSET(MAX31343_FIELD_TFS)) |
		(startImmediately << MAX31343_FIELD_OFFSET(MAX31343_FIELD_TE));

	// coinfigure timer
	status = MAX31343_WriteSingleRegister(MAX31343_REGISTER_TIMER_CONFIG, configurationRegisterValue);
	if (status) {
		return status;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Timer_Start() {
	MAX31343_Status status;
	uint8_t originalValue;
	uint8_t newValue;

	status = MAX31343_ReadSingleRegister(MAX31343_REGISTER_TIMER_CONFIG, &originalValue);
	if (status) {
		return status;
	}

	// timer is already started
	if (originalValue & MAX31343_FIELD_MASK(MAX31343_FIELD_TE)) {
		return MAX31343_Status_InvalidOperation;
	}

	// clear pause flag and set start flag
	newValue = (originalValue & ~MAX31343_FIELD_MASK(MAX31343_FIELD_TP)) | MAX31343_FIELD_MASK(MAX31343_FIELD_TE);

	status = MAX31343_WriteSingleRegister(MAX31343_REGISTER_TIMER_CONFIG, newValue);
	if (status) {
		return status;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Timer_Stop() {
	MAX31343_Status status;
	uint8_t originalValue;
	uint8_t newValue;

	status = MAX31343_ReadSingleRegister(MAX31343_REGISTER_TIMER_CONFIG, &originalValue);
	if (status) {
		return status;
	}

	// timer is already stopped
	if ((originalValue & MAX31343_FIELD_MASK(MAX31343_FIELD_TE)) == 0) {
		return MAX31343_Status_InvalidOperation;
	}

	// clear enable and pause flag
	newValue = originalValue & ~(MAX31343_FIELD_MASK(MAX31343_FIELD_TE) | MAX31343_FIELD_MASK(MAX31343_FIELD_TP));

	status = MAX31343_WriteSingleRegister(MAX31343_REGISTER_TIMER_CONFIG, newValue);
	if (status) {
		return status;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Timer_Pause() {
	MAX31343_Status status;
	uint8_t originalValue;
	uint8_t newValue;

	status = MAX31343_ReadSingleRegister(MAX31343_REGISTER_TIMER_CONFIG, &originalValue);
	if (status) {
		return status;
	}

	// timer is already paused
	if (originalValue & MAX31343_FIELD_MASK(MAX31343_FIELD_TP)) {
		return MAX31343_Status_InvalidOperation;
	}

	// timer is stopped
	if ((originalValue & MAX31343_FIELD_MASK(MAX31343_FIELD_TE)) == 0) {
		return MAX31343_Status_InvalidOperation;
	}

	// add pause flag
	newValue = originalValue | MAX31343_FIELD_MASK(MAX31343_FIELD_TP);

	status = MAX31343_WriteSingleRegister(MAX31343_REGISTER_TIMER_CONFIG, newValue);
	if (status) {
		return status;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Timer_SetRepeatMode(MAX31343_Timer_RepeatMode repeatMode) {
	if (repeatMode < 0 || repeatMode > 1) {
		return MAX31343_Status_BadArg;
	}

	return MAX31343_WriteField(MAX31343_FIELD_TRPT, repeatMode);
}

MAX31343_Status MAX31343_Timer_GetRepeatMode(MAX31343_Timer_RepeatMode* repeatMode) {
	MAX31343_Status status;
	uint8_t value;

	status = MAX31343_ReadField(MAX31343_FIELD_TRPT, &value);
	if (status) {
		return status;
	}

	*repeatMode = value;
	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Timer_SetFrequency(MAX31343_Timer_Frequency frequency) {
	if (frequency < 0 || frequency > 3) {
		return MAX31343_Status_BadArg;
	}

	return MAX31343_WriteField(MAX31343_FIELD_TFS, frequency);
}

MAX31343_Status MAX31343_Timer_GetFrequency(MAX31343_Timer_Frequency* frequency) {
	MAX31343_Status status;
	uint8_t value;

	status = MAX31343_ReadField(MAX31343_FIELD_TFS, &value);
	if (status) {
		return status;
	}

	*frequency = value;
	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Timer_GetCounter(uint8_t* value) {
	return MAX31343_ReadSingleRegister(MAX31343_REGISTER_TIMER_COUNT, value);
}

MAX31343_Status MAX31343_Timer_SetAutoReloadValue(uint8_t value) {
	return MAX31343_WriteSingleRegister(MAX31343_REGISTER_TIMER_INIT, value);
}

MAX31343_Status MAX31343_Timer_GetAutoReloadValue(uint8_t* value) {
	return MAX31343_ReadSingleRegister(MAX31343_REGISTER_TIMER_INIT, value);
}

static MAX31343_Status MAX31343_Alarm1_WriteRegisters(uint8_t registers[6]) {
	MAX31343_Status status;
	int wasAlarm1InterruptEnabled;

	status = MAX31343_ReadBoolField(MAX31343_FIELD_A1IE, &wasAlarm1InterruptEnabled);
	if (status) {
		return status;
	}

	// temporarily disable interrupt
	if (wasAlarm1InterruptEnabled) {
		status = MAX31343_WriteBoolField(MAX31343_FIELD_A1IE, 0);
		if (status) {
			return status;
		}
	}

	status = MAX31343_WriteRegisters(MAX31343_REGISTER_ALM1_SEC, registers, 6);
	if (status) {
		return status;
	}

	// enable interrupt again (only if it was enabled before)
	if (wasAlarm1InterruptEnabled) {
		status = MAX31343_WriteBoolField(MAX31343_FIELD_A1IE, 1);
		if (status) {
			return status;
		}
	}

	return MAX31343_Status_Ok;
}

static MAX31343_Status MAX31343_Alarm1_ReadRegisters(uint8_t registers[6]) {
	MAX31343_Status status;

	status = MAX31343_ReadRegisters(MAX31343_REGISTER_ALM1_SEC, registers, 6);
	if (status) {
		return status;
	}

	return MAX31343_Status_Ok;
}

static MAX31343_Status MAX31343_Alarm2_WriteRegisters(uint8_t registers[3]) {
	MAX31343_Status status;
	int wasAlarm2InterruptEnabled;

	status = MAX31343_ReadBoolField(MAX31343_FIELD_A2IE, &wasAlarm2InterruptEnabled);
	if (status) {
		return status;
	}

	// temporarily disable interrupt
	if (wasAlarm2InterruptEnabled) {
		status = MAX31343_WriteBoolField(MAX31343_FIELD_A2IE, 0);
		if (status) {
			return status;
		}
	}

	status = MAX31343_WriteRegisters(MAX31343_REGISTER_ALM2_MIN, registers, 3);
	if (status) {
		return status;
	}

	// enable interrupt again (only if it was enabled before)
	if (wasAlarm2InterruptEnabled) {
		status = MAX31343_WriteBoolField(MAX31343_FIELD_A2IE, 1);
		if (status) {
			return status;
		}
	}

	return MAX31343_Status_Ok;
}

static MAX31343_Status MAX31343_Alarm2_ReadRegisters(uint8_t registers[3]) {
	MAX31343_Status status;

	status = MAX31343_ReadRegisters(MAX31343_REGISTER_ALM2_MIN, registers, 3);
	if (status) {
		return status;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Alarm1_ConfigureEverySecond() {
	uint8_t registers[6];

	registers[0] = MAX31343_FIELD_MASK(MAX31343_FIELD_A1M1);
	registers[1] = MAX31343_FIELD_MASK(MAX31343_FIELD_A1M2);
	registers[2] = MAX31343_FIELD_MASK(MAX31343_FIELD_A1M3);
	registers[3] = MAX31343_FIELD_MASK(MAX31343_FIELD_A1M4);
	registers[4] = MAX31343_FIELD_MASK(MAX31343_FIELD_A1M5) | MAX31343_FIELD_MASK(MAX31343_FIELD_A1M6);
	registers[5] = 0;

	return MAX31343_Alarm1_WriteRegisters(registers);
}

MAX31343_Status MAX31343_Alarm1_ConfigureEveryMinute(uint8_t atSecond, int isNumberInBcdFormat) {
	uint8_t registers[6];

	if (isNumberInBcdFormat) {
		registers[0] = atSecond;
	} else {
		registers[0] = MAX31343_VALUE_TO_BCD(atSecond);
	}
	registers[1] = MAX31343_FIELD_MASK(MAX31343_FIELD_A1M2);
	registers[2] = MAX31343_FIELD_MASK(MAX31343_FIELD_A1M3);
	registers[3] = MAX31343_FIELD_MASK(MAX31343_FIELD_A1M4);
	registers[4] = MAX31343_FIELD_MASK(MAX31343_FIELD_A1M5) | MAX31343_FIELD_MASK(MAX31343_FIELD_A1M6);
	registers[5] = 0;

	return MAX31343_Alarm1_WriteRegisters(registers);
}

MAX31343_Status MAX31343_Alarm1_ConfigureEveryHour(uint8_t atMinute, uint8_t atSecond, int areNumbersInBcdFormat) {
	uint8_t registers[6];

	if (areNumbersInBcdFormat) {
		registers[0] = atSecond;
		registers[1] = atMinute;
	} else {
		registers[0] = MAX31343_VALUE_TO_BCD(atSecond);
		registers[1] = MAX31343_VALUE_TO_BCD(atMinute);
	}
	registers[2] = MAX31343_FIELD_MASK(MAX31343_FIELD_A1M3);
	registers[3] = MAX31343_FIELD_MASK(MAX31343_FIELD_A1M4);
	registers[4] = MAX31343_FIELD_MASK(MAX31343_FIELD_A1M5) | MAX31343_FIELD_MASK(MAX31343_FIELD_A1M6);
	registers[5] = 0;

	return MAX31343_Alarm1_WriteRegisters(registers);
}

MAX31343_Status MAX31343_Alarm1_ConfigureEveryDay(uint8_t atHour, uint8_t atMinute, uint8_t atSecond, int areNumbersInBcdFormat) {
	uint8_t registers[6];

	if (areNumbersInBcdFormat) {
		registers[0] = atSecond;
		registers[1] = atMinute;
		registers[2] = atHour;
	} else {
		registers[0] = MAX31343_VALUE_TO_BCD(atSecond);
		registers[1] = MAX31343_VALUE_TO_BCD(atMinute);
		registers[2] = MAX31343_VALUE_TO_BCD(atHour);
	}
	registers[3] = MAX31343_FIELD_MASK(MAX31343_FIELD_A1M4);
	registers[4] = MAX31343_FIELD_MASK(MAX31343_FIELD_A1M5) | MAX31343_FIELD_MASK(MAX31343_FIELD_A1M6);
	registers[5] = 0;

	return MAX31343_Alarm1_WriteRegisters(registers);
}

MAX31343_Status MAX31343_Alarm1_ConfigureEveryWeek(uint8_t atDayOfWeek, uint8_t atHour, uint8_t atMinute, uint8_t atSecond, int areNumbersInBcdFormat) {
	uint8_t registers[6];

	if (areNumbersInBcdFormat) {
		registers[0] = atSecond;
		registers[1] = atMinute;
		registers[2] = atHour;
	} else {
		registers[0] = MAX31343_VALUE_TO_BCD(atSecond);
		registers[1] = MAX31343_VALUE_TO_BCD(atMinute);
		registers[2] = MAX31343_VALUE_TO_BCD(atHour);
	}
	registers[3] = atDayOfWeek | MAX31343_FIELD_MASK(MAX31343_FIELD_A1DY_DT);
	registers[4] = MAX31343_FIELD_MASK(MAX31343_FIELD_A1M5) | MAX31343_FIELD_MASK(MAX31343_FIELD_A1M6);
	registers[5] = 0;

	return MAX31343_Alarm1_WriteRegisters(registers);
}

MAX31343_Status MAX31343_Alarm1_ConfigureEveryMonth(uint8_t atDate, uint8_t atHour, uint8_t atMinute, uint8_t atSecond, int areNumbersInBcdFormat) {
	uint8_t registers[6];

	if (areNumbersInBcdFormat) {
		registers[0] = atSecond;
		registers[1] = atMinute;
		registers[2] = atHour;
		registers[3] = atDate;
	} else {
		registers[0] = MAX31343_VALUE_TO_BCD(atSecond);
		registers[1] = MAX31343_VALUE_TO_BCD(atMinute);
		registers[2] = MAX31343_VALUE_TO_BCD(atHour);
		registers[3] = MAX31343_VALUE_TO_BCD(atDate);
	}
	registers[4] = MAX31343_FIELD_MASK(MAX31343_FIELD_A1M5) | MAX31343_FIELD_MASK(MAX31343_FIELD_A1M6);
	registers[5] = 0;

	return MAX31343_Alarm1_WriteRegisters(registers);
}

MAX31343_Status MAX31343_Alarm1_ConfigureEveryYear(uint8_t atMonth, uint8_t atDate, uint8_t atHour, uint8_t atMinute, uint8_t atSecond, int areNumbersInBcdFormat) {
	uint8_t registers[6];

	if (areNumbersInBcdFormat) {
		registers[0] = atSecond;
		registers[1] = atMinute;
		registers[2] = atHour;
		registers[3] = atDate;
		registers[4] = atMonth | MAX31343_FIELD_MASK(MAX31343_FIELD_A1M6);
	} else {
		registers[0] = MAX31343_VALUE_TO_BCD(atSecond);
		registers[1] = MAX31343_VALUE_TO_BCD(atMinute);
		registers[2] = MAX31343_VALUE_TO_BCD(atHour);
		registers[3] = MAX31343_VALUE_TO_BCD(atDate);
		registers[4] = MAX31343_VALUE_TO_BCD(atMonth) | MAX31343_FIELD_MASK(MAX31343_FIELD_A1M6);
	}
	registers[5] = 0;

	return MAX31343_Alarm1_WriteRegisters(registers);
}

MAX31343_Status MAX31343_Alarm1_ConfigureOnce(uint8_t atYear, uint8_t atMonth, uint8_t atDate, uint8_t atHour, uint8_t atMinute, uint8_t atSecond, int areNumbersInBcdFormat) {
	uint8_t registers[6];

	if (areNumbersInBcdFormat) {
		registers[0] = atSecond;
		registers[1] = atMinute;
		registers[2] = atHour;
		registers[3] = atDate;
		registers[4] = atMonth;
		registers[5] = atYear;
	} else {
		registers[0] = MAX31343_VALUE_TO_BCD(atSecond);
		registers[1] = MAX31343_VALUE_TO_BCD(atMinute);
		registers[2] = MAX31343_VALUE_TO_BCD(atHour);
		registers[3] = MAX31343_VALUE_TO_BCD(atDate);
		registers[4] = MAX31343_VALUE_TO_BCD(atMonth);
		registers[5] = MAX31343_VALUE_TO_BCD(atYear);
	}

	return MAX31343_Alarm1_WriteRegisters(registers);
}

static MAX31343_Status MAX31343_Alarm1_GetAlarmTypeFromRegisters(uint8_t registers[6], MAX31343_AlarmType* alarmType) {

	/*
	 * bit 0 - A1M1
	 * bit 1 - A1M2
	 * bit 2 - A1M3
	 * bit 3 - A1M4
	 * bit 4 - A1M5
	 * bit 5 - A1M6
	 * bit 7 - A1DY_DT
	 */
	uint8_t maskBits = 0;

	if (registers[0] & MAX31343_FIELD_MASK(MAX31343_FIELD_A1M1)) {
		maskBits |= (1 << 0);
	}
	if (registers[1] & MAX31343_FIELD_MASK(MAX31343_FIELD_A1M2)) {
		maskBits |= (1 << 1);
	}
	if (registers[2] & MAX31343_FIELD_MASK(MAX31343_FIELD_A1M3)) {
		maskBits |= (1 << 2);
	}
	if (registers[3] & MAX31343_FIELD_MASK(MAX31343_FIELD_A1M4)) {
		maskBits |= (1 << 3);
	}
	if (registers[4] & MAX31343_FIELD_MASK(MAX31343_FIELD_A1M5)) {
		maskBits |= (1 << 4);
	}
	if (registers[4] & MAX31343_FIELD_MASK(MAX31343_FIELD_A1M6)) {
		maskBits |= (1 << 5);
	}
	if (registers[3] & MAX31343_FIELD_MASK(MAX31343_FIELD_A1DY_DT)) {
		maskBits |= (1 << 7);
	}

	if (maskBits == 0b00111111 ||
		maskBits == 0b10111111) {
		*alarmType = MAX31343_AlarmType_EverySecond;
	} else if (
		maskBits == 0b00111110 ||
		maskBits == 0b10111110) {
		*alarmType = MAX31343_AlarmType_EveryMinute;
	} else if (
		maskBits == 0b00111100 ||
		maskBits == 0b10111100) {
		*alarmType = MAX31343_AlarmType_EveryHour;
	} else if (
		maskBits == 0b00111000 ||
		maskBits == 0b10111000) {
		*alarmType = MAX31343_AlarmType_EveryDay;
	} else if (
		maskBits == 0b00110000) {
		*alarmType = MAX31343_AlarmType_EveryMonth;
	} else if (
		maskBits == 0b10110000) {
		*alarmType = MAX31343_AlarmType_EveryWeek;
	} else if (
		maskBits == 0b00100000) {
		*alarmType = MAX31343_AlarmType_EveryYear;
	} else if (
		maskBits == 0b00000000) {
		*alarmType = MAX31343_AlarmType_Once;
	} else {
		*alarmType = MAX31343_AlarmType_NonStandard;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Alarm1_GetAlarmType(MAX31343_AlarmType* alarmType) {
	MAX31343_Status status;

	uint8_t registers[6];

	status = MAX31343_Alarm1_ReadRegisters(registers);
	if (status) {
		return status;
	}

	return MAX31343_Alarm1_GetAlarmTypeFromRegisters(registers, alarmType);

}

MAX31343_Status MAX31343_Alarm1_GetConfiguration(MAX31343_AlarmType* alarmType, uint8_t* atYear, uint8_t* atMonth, uint8_t* atDate, uint8_t* atDayOfWeek, uint8_t* atHour, uint8_t* atMinute, uint8_t* atSecond, int getNumbersInBcdFormat) {
	MAX31343_Status status;

	uint8_t registers[6];

	status = MAX31343_Alarm1_ReadRegisters(registers);
	if (status) {
		return status;
	}

	status = MAX31343_Alarm1_GetAlarmTypeFromRegisters(registers, alarmType);
	if (status) {
		return status;
	}

	// parse second from alarm configuration
	if (*alarmType == MAX31343_AlarmType_EveryMinute ||
		*alarmType == MAX31343_AlarmType_EveryHour ||
		*alarmType == MAX31343_AlarmType_EveryDay ||
		*alarmType == MAX31343_AlarmType_EveryWeek ||
		*alarmType == MAX31343_AlarmType_EveryMonth ||
		*alarmType == MAX31343_AlarmType_EveryYear ||
		*alarmType == MAX31343_AlarmType_Once) {

		if (atSecond == NULL) {
			return MAX31343_Status_BadArg;
		}

		uint8_t sec = registers[0] & ~MAX31343_FIELD_MASK(MAX31343_FIELD_A1M1);
		if (getNumbersInBcdFormat) {
			*atSecond = sec;
		} else {
			*atSecond = MAX31343_BCD_TO_VALUE(sec);
		}
	}

	// parse minute from alarm configuration
	if (*alarmType == MAX31343_AlarmType_EveryHour ||
		*alarmType == MAX31343_AlarmType_EveryDay ||
		*alarmType == MAX31343_AlarmType_EveryWeek ||
		*alarmType == MAX31343_AlarmType_EveryMonth ||
		*alarmType == MAX31343_AlarmType_EveryYear ||
		*alarmType == MAX31343_AlarmType_Once) {

		if (atMinute == NULL) {
			return MAX31343_Status_BadArg;
		}

		uint8_t min = registers[1] & ~MAX31343_FIELD_MASK(MAX31343_FIELD_A1M2);
		if (getNumbersInBcdFormat) {
			*atMinute = min;
		} else {
			*atMinute = MAX31343_BCD_TO_VALUE(min);
		}
	}

	// parse hour from alarm configuration
	if (*alarmType == MAX31343_AlarmType_EveryDay ||
		*alarmType == MAX31343_AlarmType_EveryWeek ||
		*alarmType == MAX31343_AlarmType_EveryMonth ||
		*alarmType == MAX31343_AlarmType_EveryYear ||
		*alarmType == MAX31343_AlarmType_Once) {

		if (atHour == NULL) {
			return MAX31343_Status_BadArg;
		}

		uint8_t hour = registers[2] & ~MAX31343_FIELD_MASK(MAX31343_FIELD_A1M3);
		if (getNumbersInBcdFormat) {
			*atHour = hour;
		} else {
			*atHour = MAX31343_BCD_TO_VALUE(hour);
		}
	}

	// parse date from alarm configuration
	if (*alarmType == MAX31343_AlarmType_EveryMonth ||
		*alarmType == MAX31343_AlarmType_EveryYear ||
		*alarmType == MAX31343_AlarmType_Once) {

		if (atDate == NULL) {
			return MAX31343_Status_BadArg;
		}

		uint8_t date = registers[3] &
			~(MAX31343_FIELD_MASK(MAX31343_FIELD_A1M4) |
				MAX31343_FIELD_MASK(MAX31343_FIELD_A1DY_DT));

		if (getNumbersInBcdFormat) {
			*atDate = date;
		} else {
			*atDate = MAX31343_BCD_TO_VALUE(date);
		}
	}

	// parse day of week from alarm configuration
	if (*alarmType == MAX31343_AlarmType_EveryWeek) {

		if (atDayOfWeek == NULL) {
			return MAX31343_Status_BadArg;
		}

		*atDayOfWeek = registers[3] &
			~(MAX31343_FIELD_MASK(MAX31343_FIELD_A1M4) |
				MAX31343_FIELD_MASK(MAX31343_FIELD_A1DY_DT));
	}

	// parse month from alarm configuration
	if (*alarmType == MAX31343_AlarmType_EveryYear ||
		*alarmType == MAX31343_AlarmType_Once) {

		if (atMonth == NULL) {
			return MAX31343_Status_BadArg;
		}

		uint8_t month = registers[4] & ~(MAX31343_FIELD_MASK(MAX31343_FIELD_A1M5) | MAX31343_FIELD_MASK(MAX31343_FIELD_A1M6));
		if (getNumbersInBcdFormat) {
			*atMonth = month;
		} else {
			*atMonth = MAX31343_BCD_TO_VALUE(month);
		}
	}

	// parse year from alarm configuration
	if (*alarmType == MAX31343_AlarmType_Once) {

		if (atYear == NULL) {
			return MAX31343_Status_BadArg;
		}

		uint8_t year = registers[5];
		if (getNumbersInBcdFormat) {
			*atYear = year;
		} else {
			*atYear = MAX31343_BCD_TO_VALUE(year);
		}
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Alarm1_GetEveryMinuteConfiguration(uint8_t* atSecond, int getNumbersInBcdFormat) {
	MAX31343_Status status;
	MAX31343_AlarmType alarmType;
	uint8_t dummy;

	status = MAX31343_Alarm1_GetConfiguration(&alarmType, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy, atSecond, getNumbersInBcdFormat);
	if (status) {
		return status;
	}

	if (alarmType != MAX31343_AlarmType_EveryMinute) {
		return MAX31343_Status_InvalidOperation;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Alarm1_GetEveryHourConfiguration(uint8_t* atMinute, uint8_t* atSecond, int getNumbersInBcdFormat) {
	MAX31343_Status status;
	MAX31343_AlarmType alarmType;
	uint8_t dummy;

	status = MAX31343_Alarm1_GetConfiguration(&alarmType, &dummy, &dummy, &dummy, &dummy, &dummy, atMinute, atSecond, getNumbersInBcdFormat);
	if (status) {
		return status;
	}

	if (alarmType != MAX31343_AlarmType_EveryHour) {
		return MAX31343_Status_InvalidOperation;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Alarm1_GetEveryDayConfiguration(uint8_t* atHour, uint8_t* atMinute, uint8_t* atSecond, int getNumbersInBcdFormat) {
	MAX31343_Status status;
	MAX31343_AlarmType alarmType;
	uint8_t dummy;

	status = MAX31343_Alarm1_GetConfiguration(&alarmType, &dummy, &dummy, &dummy, &dummy, atHour, atMinute, atSecond, getNumbersInBcdFormat);
	if (status) {
		return status;
	}

	if (alarmType != MAX31343_AlarmType_EveryDay) {
		return MAX31343_Status_InvalidOperation;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Alarm1_GetEveryWeekConfiguration(uint8_t* atDayOfWeek, uint8_t* atHour, uint8_t* atMinute, uint8_t* atSecond, int getNumbersInBcdFormat) {
	MAX31343_Status status;
	MAX31343_AlarmType alarmType;
	uint8_t dummy;

	status = MAX31343_Alarm1_GetConfiguration(&alarmType, &dummy, &dummy, &dummy, atDayOfWeek, atHour, atMinute, atSecond, getNumbersInBcdFormat);
	if (status) {
		return status;
	}

	if (alarmType != MAX31343_AlarmType_EveryWeek) {
		return MAX31343_Status_InvalidOperation;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Alarm1_GetEveryMonthConfiguration(uint8_t* atDate, uint8_t* atHour, uint8_t* atMinute, uint8_t* atSecond, int getNumbersInBcdFormat) {
	MAX31343_Status status;
	MAX31343_AlarmType alarmType;
	uint8_t dummy;

	status = MAX31343_Alarm1_GetConfiguration(&alarmType, &dummy, &dummy, atDate, &dummy, atHour, atMinute, atSecond, getNumbersInBcdFormat);
	if (status) {
		return status;
	}

	if (alarmType != MAX31343_AlarmType_EveryMonth) {
		return MAX31343_Status_InvalidOperation;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Alarm1_GetEveryYearConfiguration(uint8_t* atMonth, uint8_t* atDate, uint8_t* atHour, uint8_t* atMinute, uint8_t* atSecond, int getNumbersInBcdFormat) {
	MAX31343_Status status;
	MAX31343_AlarmType alarmType;
	uint8_t dummy;

	status = MAX31343_Alarm1_GetConfiguration(&alarmType, &dummy, atMonth, atDate, &dummy, atHour, atMinute, atSecond, getNumbersInBcdFormat);
	if (status) {
		return status;
	}

	if (alarmType != MAX31343_AlarmType_EveryYear) {
		return MAX31343_Status_InvalidOperation;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Alarm1_GetOnceConfiguration(uint8_t* atYear, uint8_t* atMonth, uint8_t* atDate, uint8_t* atHour, uint8_t* atMinute, uint8_t* atSecond, int getNumbersInBcdFormat) {
	MAX31343_Status status;
	MAX31343_AlarmType alarmType;
	uint8_t dummy;

	status = MAX31343_Alarm1_GetConfiguration(&alarmType, atYear, atMonth, atDate, &dummy, atHour, atMinute, atSecond, getNumbersInBcdFormat);
	if (status) {
		return status;
	}

	if (alarmType != MAX31343_AlarmType_Once) {
		return MAX31343_Status_InvalidOperation;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Alarm2_ConfigureEveryMinute() {
	uint8_t registers[3];

	registers[0] = MAX31343_FIELD_MASK(MAX31343_FIELD_A2M2);
	registers[1] = MAX31343_FIELD_MASK(MAX31343_FIELD_A2M3);
	registers[2] = MAX31343_FIELD_MASK(MAX31343_FIELD_A2M4);

	return MAX31343_Alarm2_WriteRegisters(registers);
}

MAX31343_Status MAX31343_Alarm2_ConfigureEveryHour(uint8_t atMinute, int isNumberInBcdFormat) {
	uint8_t registers[3];

	if (isNumberInBcdFormat) {
		registers[0] = atMinute;
	} else {
		registers[0] = MAX31343_VALUE_TO_BCD(atMinute);
	}
	registers[1] = MAX31343_FIELD_MASK(MAX31343_FIELD_A2M3);
	registers[2] = MAX31343_FIELD_MASK(MAX31343_FIELD_A2M4);

	return MAX31343_Alarm2_WriteRegisters(registers);
}

MAX31343_Status MAX31343_Alarm2_ConfigureEveryDay(uint8_t atHour, uint8_t atMinute, int areNumbersInBcdFormat) {
	uint8_t registers[3];

	if (areNumbersInBcdFormat) {
		registers[0] = atMinute;
		registers[1] = atHour;
	} else {
		registers[0] = MAX31343_VALUE_TO_BCD(atMinute);
		registers[1] = MAX31343_VALUE_TO_BCD(atHour);
	}
	registers[2] = MAX31343_FIELD_MASK(MAX31343_FIELD_A2M4);

	return MAX31343_Alarm2_WriteRegisters(registers);
}

MAX31343_Status MAX31343_Alarm2_ConfigureEveryWeek(uint8_t atDayOfWeek, uint8_t atHour, uint8_t atMinute, int areNumbersInBcdFormat) {
	uint8_t registers[3];

	if (areNumbersInBcdFormat) {
		registers[0] = atMinute;
		registers[1] = atHour;
	} else {
		registers[0] = MAX31343_VALUE_TO_BCD(atMinute);
		registers[1] = MAX31343_VALUE_TO_BCD(atHour);
	}
	registers[2] = atDayOfWeek | MAX31343_FIELD_MASK(MAX31343_FIELD_A2DY_DT);

	return MAX31343_Alarm2_WriteRegisters(registers);
}

MAX31343_Status MAX31343_Alarm2_ConfigureEveryMonth(uint8_t atDate, uint8_t atHour, uint8_t atMinute, int areNumbersInBcdFormat) {
	uint8_t registers[3];

	if (areNumbersInBcdFormat) {
		registers[0] = atMinute;
		registers[1] = atHour;
		registers[2] = atDate;
	} else {
		registers[0] = MAX31343_VALUE_TO_BCD(atMinute);
		registers[1] = MAX31343_VALUE_TO_BCD(atHour);
		registers[2] = MAX31343_VALUE_TO_BCD(atDate);
	}

	return MAX31343_Alarm2_WriteRegisters(registers);
}

static MAX31343_Status MAX31343_Alarm2_GetAlarmTypeFromRegisters(uint8_t registers[3], MAX31343_AlarmType* alarmType) {

	/*
	 * bit 0 - A2M2
	 * bit 1 - A2M3
	 * bit 2 - A2M4
	 * bit 7 - A1DY_DT
	 */
	uint8_t maskBits = 0;

	if (registers[0] & MAX31343_FIELD_MASK(MAX31343_FIELD_A2M2)) {
		maskBits |= (1 << 0);
	}
	if (registers[1] & MAX31343_FIELD_MASK(MAX31343_FIELD_A2M3)) {
		maskBits |= (1 << 1);
	}
	if (registers[2] & MAX31343_FIELD_MASK(MAX31343_FIELD_A2M4)) {
		maskBits |= (1 << 2);
	}
	if (registers[2] & MAX31343_FIELD_MASK(MAX31343_FIELD_A1DY_DT)) {
		maskBits |= (1 << 7);
	}

	if (maskBits == 0b00000111 ||
		maskBits == 0b10000111) {
		*alarmType = MAX31343_AlarmType_EveryMinute;
	} else if (
		maskBits == 0b00000110 ||
		maskBits == 0b10000110) {
		*alarmType = MAX31343_AlarmType_EveryHour;
	} else if (
		maskBits == 0b00000100 ||
		maskBits == 0b10000100) {
		*alarmType = MAX31343_AlarmType_EveryDay;
	} else if (
		maskBits == 0b00000000) {
		*alarmType = MAX31343_AlarmType_EveryMonth;
	} else if (
		maskBits == 0b10000000) {
		*alarmType = MAX31343_AlarmType_EveryWeek;
	} else {
		*alarmType = MAX31343_AlarmType_NonStandard;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Alarm2_GetAlarmType(MAX31343_AlarmType* alarmType) {
	MAX31343_Status status;

	uint8_t registers[3];

	status = MAX31343_Alarm2_ReadRegisters(registers);
	if (status) {
		return status;
	}

	return MAX31343_Alarm2_GetAlarmTypeFromRegisters(registers, alarmType);
}

MAX31343_Status MAX31343_Alarm2_GetConfiguration(MAX31343_AlarmType* alarmType, uint8_t* atDate, uint8_t* atDayOfWeek, uint8_t* atHour, uint8_t* atMinute, int getNumbersInBcdFormat) {
	MAX31343_Status status;

	uint8_t registers[3];

	status = MAX31343_Alarm2_ReadRegisters(registers);
	if (status) {
		return status;
	}

	status = MAX31343_Alarm2_GetAlarmTypeFromRegisters(registers, alarmType);
	if (status) {
		return status;
	}

	// parse minute from alarm configuration
	if (*alarmType == MAX31343_AlarmType_EveryHour ||
		*alarmType == MAX31343_AlarmType_EveryDay ||
		*alarmType == MAX31343_AlarmType_EveryWeek ||
		*alarmType == MAX31343_AlarmType_EveryMonth) {

		if (atMinute == NULL) {
			return MAX31343_Status_BadArg;
		}

		uint8_t min = registers[0] & ~MAX31343_FIELD_MASK(MAX31343_FIELD_A2M2);
		if (getNumbersInBcdFormat) {
			*atMinute = min;
		} else {
			*atMinute = MAX31343_BCD_TO_VALUE(min);
		}
	}

	// parse hour from alarm configuration
	if (*alarmType == MAX31343_AlarmType_EveryDay ||
		*alarmType == MAX31343_AlarmType_EveryWeek ||
		*alarmType == MAX31343_AlarmType_EveryMonth) {

		if (atHour == NULL) {
			return MAX31343_Status_BadArg;
		}

		uint8_t hour = registers[1] & ~MAX31343_FIELD_MASK(MAX31343_FIELD_A2M3);
		if (getNumbersInBcdFormat) {
			*atHour = hour;
		} else {
			*atHour = MAX31343_BCD_TO_VALUE(hour);
		}
	}

	// parse date from alarm configuration
	if (*alarmType == MAX31343_AlarmType_EveryMonth) {

		if (atDate == NULL) {
			return MAX31343_Status_BadArg;
		}

		uint8_t date = registers[2] &
			~(MAX31343_FIELD_MASK(MAX31343_FIELD_A2M4) |
				MAX31343_FIELD_MASK(MAX31343_FIELD_A2DY_DT));

		if (getNumbersInBcdFormat) {
			*atDate = date;
		} else {
			*atDate = MAX31343_BCD_TO_VALUE(date);
		}
	}

	// parse day of week from alarm configuration
	if (*alarmType == MAX31343_AlarmType_EveryWeek) {

		if (atDayOfWeek == NULL) {
			return MAX31343_Status_BadArg;
		}

		*atDayOfWeek = registers[2] &
			~(MAX31343_FIELD_MASK(MAX31343_FIELD_A2M4) |
				MAX31343_FIELD_MASK(MAX31343_FIELD_A2DY_DT));
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Alarm2_GetEveryHourConfiguration(uint8_t* atMinute, int getNumbersInBcdFormat) {
	MAX31343_Status status;
	MAX31343_AlarmType alarmType;
	uint8_t dummy;

	status = MAX31343_Alarm2_GetConfiguration(&alarmType, &dummy, &dummy, &dummy, atMinute, getNumbersInBcdFormat);
	if (status) {
		return status;
	}

	if (alarmType != MAX31343_AlarmType_EveryHour) {
		return MAX31343_Status_InvalidOperation;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Alarm2_GetEveryDayConfiguration(uint8_t* atHour, uint8_t* atMinute, int getNumbersInBcdFormat) {
	MAX31343_Status status;
	MAX31343_AlarmType alarmType;
	uint8_t dummy;

	status = MAX31343_Alarm2_GetConfiguration(&alarmType, &dummy, &dummy, atHour, atMinute, getNumbersInBcdFormat);
	if (status) {
		return status;
	}

	if (alarmType != MAX31343_AlarmType_EveryDay) {
		return MAX31343_Status_InvalidOperation;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Alarm2_GetEveryWeekConfiguration(uint8_t* atDayOfWeek, uint8_t* atHour, uint8_t* atMinute, int getNumbersInBcdFormat) {
	MAX31343_Status status;
	MAX31343_AlarmType alarmType;
	uint8_t dummy;

	status = MAX31343_Alarm2_GetConfiguration(&alarmType, &dummy, atDayOfWeek, atHour, atMinute, getNumbersInBcdFormat);
	if (status) {
		return status;
	}

	if (alarmType != MAX31343_AlarmType_EveryWeek) {
		return MAX31343_Status_InvalidOperation;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_Alarm2_GetEveryMonthConfiguration(uint8_t* atDate, uint8_t* atHour, uint8_t* atMinute, int getNumbersInBcdFormat) {
	MAX31343_Status status;
	MAX31343_AlarmType alarmType;
	uint8_t dummy;

	status = MAX31343_Alarm2_GetConfiguration(&alarmType, atDate, &dummy, atHour, atMinute, getNumbersInBcdFormat);
	if (status) {
		return status;
	}

	if (alarmType != MAX31343_AlarmType_EveryMonth) {
		return MAX31343_Status_InvalidOperation;
	}

	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_TemperatureSensor_GetTemperatureRaw(uint16_t* temperature) {
	MAX31343_Status status;
	uint8_t registerValues[2];

	status = MAX31343_ReadRegisters(MAX31343_REGISTER_TEMP_MSB, registerValues, sizeof(registerValues));
	if (status) {
		return status;
	}

	*temperature = (((uint16_t)registerValues[0]) << 8) | ((uint16_t)registerValues[0]);
	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_TemperatureSensor_GetTemperatureFloat(float* temperature) {
	MAX31343_Status status;
	uint16_t temperatureRaw;

	status = MAX31343_TemperatureSensor_GetTemperatureRaw(&temperatureRaw);
	if (status) {
		return status;
	}

	int8_t wholePart = (int8_t)((temperatureRaw & 0xFF00) >> 8);
	uint8_t decimalPart = (temperatureRaw & 0xFF) >> 6;

	*temperature = wholePart + (float)decimalPart / 4;
	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_TemperatureSensor_EnableAutomaticMode() {
	return MAX31343_WriteBoolField(MAX31343_FIELD_AUTOMODE, 1);
}

MAX31343_Status MAX31343_TemperatureSensor_DisableAutomaticMode() {
	return MAX31343_WriteBoolField(MAX31343_FIELD_AUTOMODE, 0);
}

MAX31343_Status MAX31343_TemperatureSensor_IsAutomaticModeEnabled(int* isAutomaticModeEnabled) {
	return MAX31343_ReadBoolField(MAX31343_FIELD_AUTOMODE, isAutomaticModeEnabled);
}

MAX31343_Status MAX31343_TemperatureSensor_TriggerOneShotConversion() {
	return MAX31343_WriteBoolField(MAX31343_FIELD_ONESHOTMODE, 1);
}

MAX31343_Status MAX31343_TemperatureSensor_IsOneShotConversionDone(int* isOneShotConversionDone) {
	return MAX31343_ReadBoolField(MAX31343_FIELD_TSF, isOneShotConversionDone);
}

MAX31343_Status MAX31343_TemperatureSensor_SetAutomaticModeInterval(MAX31343_TemperatureSensor_AutomaticModeInterval interval) {
	if (interval < 0 || interval > 7) {
		return MAX31343_Status_BadArg;
	}

	return MAX31343_WriteField(MAX31343_FIELD_TTSINT, interval);
}

MAX31343_Status MAX31343_TemperatureSensor_EnableAutomaticModeAndSetInterval(MAX31343_TemperatureSensor_AutomaticModeInterval interval) {
	if (interval < 0 || interval > 7) {
		return MAX31343_Status_BadArg;
	}

	uint8_t regVal = (interval << MAX31343_FIELD_OFFSET(MAX31343_FIELD_TTSINT)) | MAX31343_FIELD_MASK(MAX31343_FIELD_AUTOMODE);

	return MAX31343_WriteSingleRegister(MAX31343_REGISTER_TS_CONFIG, regVal);
}

MAX31343_Status MAX31343_TemperatureSensor_GetAutomaticModeInterval(MAX31343_TemperatureSensor_AutomaticModeInterval* interval) {
	MAX31343_Status status;
	uint8_t value;

	status = MAX31343_ReadField(MAX31343_FIELD_TTSINT, &value);
	if (status) {
		return status;
	}

	*interval = value;
	return MAX31343_Status_Ok;
}

MAX31343_Status MAX31343_ReadRam(uint8_t offset, uint8_t* buffer, size_t bufferSize) {
	if (offset > MAX31343_RAM_SIZE) {
		return MAX31343_Status_BadArg;
	}

	if ((size_t)offset + bufferSize > MAX31343_RAM_SIZE) {
		return MAX31343_Status_BadArg;
	}

	if (bufferSize == 0) {
		return MAX31343_Status_Ok;
	}

	if (buffer == NULL) {
		return MAX31343_Status_BadArg;
	}

	return MAX31343_ReadRegisters(MAX31343_REGISTER_RAM_REG, buffer, bufferSize);
}

MAX31343_Status MAX31343_WriteRam(uint8_t offset, uint8_t* buffer, size_t bufferSize) {
	if (offset > MAX31343_RAM_SIZE) {
		return MAX31343_Status_BadArg;
	}

	if ((size_t)offset + bufferSize > MAX31343_RAM_SIZE) {
		return MAX31343_Status_BadArg;
	}

	if (bufferSize == 0) {
		return MAX31343_Status_Ok;
	}

	if (buffer == NULL) {
		return MAX31343_Status_BadArg;
	}

	return MAX31343_WriteRegisters(MAX31343_REGISTER_RAM_REG, buffer, bufferSize);
}
