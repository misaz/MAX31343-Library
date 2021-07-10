#ifndef __MAX31343_H
#define __MAX31343_H

#include <stdint.h>
#include <stddef.h>

//#define MAX3134_STRUCT_PACKED __attribute__((packed))
#define MAX3134_STRUCT_PACKED 

typedef enum {
	MAX31343_Status_Ok = 0,
	MAX31343_Status_I2CNack = -10,
	MAX31343_Status_I2CError = -11,
	MAX31343_Status_I2CTimeout = -12,
	MAX31343_Status_NotImplemented = -20,
	MAX31343_Status_BadArg = -21,
	MAX31343_Status_InvalidOperation = -22,
} MAX31343_Status;

typedef enum {
	MAX31343_Interrupt_Alarm1 = 1,
	MAX31343_Interrupt_Alarm2 = 2,
	MAX31343_Interrupt_Timer = 4,
	MAX31343_Interrupt_TemperatureSensor = 8,
	MAX31343_Interrupt_PowerFail = 32,
	MAX31343_Interrupt_DisableOscilator = 64,
} MAX31343_Interrupt;

typedef enum {
	MAX31343_SqwFrequency_1Hz = 0x0,
	MAX31343_SqwFrequency_2Hz = 0x1,
	MAX31343_SqwFrequency_4Hz = 0x2,
	MAX31343_SqwFrequency_8Hz = 0x3,
	MAX31343_SqwFrequency_16Hz = 0x4,
	MAX31343_SqwFrequency_32Hz = 0x5,
} MAX31343_SqwFrequency;

typedef enum {
	MAX31343_ClkoFrequency_1Hz = 0x0,
	MAX31343_ClkoFrequency_2Hz = 0x1,
	MAX31343_ClkoFrequency_4Hz = 0x2,
	MAX31343_ClkoFrequency_8Hz = 0x3,
	MAX31343_ClkoFrequency_16Hz = 0x4,
	MAX31343_ClkoFrequency_32Hz = 0x5,
	MAX31343_ClkoFrequency_64Hz = 0x6,
	MAX31343_ClkoFrequency_128Hz = 0x7,
	MAX31343_ClkoFrequency_32kHz = 0x8,
} MAX31343_ClkoFrequency;

typedef enum {
	MAX31343_Timer_RepeatMode_OneShot = 0x0,
	MAX31343_Timer_RepeatMode_AutoReloading = 0x1,
} MAX31343_Timer_RepeatMode;

typedef enum {
	MAX31343_Timer_Frequency_1024Hz = 0x0,
	MAX31343_Timer_Frequency_256Hz = 0x1,
	MAX31343_Timer_Frequency_64Hz = 0x2,
	MAX31343_Timer_Frequency_16Hz = 0x3,
} MAX31343_Timer_Frequency;

typedef enum {
	MAX31343_PowerSupplySelectionMode_Automatic = 0x0,
	MAX31343_PowerSupplySelectionMode_Manual = 0x1,
} MAX31343_PowerSupplySelectionMode;

typedef enum {
	MAX31343_PowerFailTreshold_Reserved = 0x0,
	MAX31343_PowerFailTreshold_1V8 = 0x1,
	MAX31343_PowerFailTreshold_2V0 = 0x2,
	MAX31343_PowerFailTreshold_2V4 = 0x3,
} MAX31343_PowerFailTreshold;

typedef enum {
	MAX31343_PowerSupplyType_Main = 0x0,
	MAX31343_PowerSupplyType_Backup = 0x1,
} MAX31343_PowerSupplyType;

typedef enum {
	MAX31343_TrickleChargerMode_3k_without_diode = 0x0,
	MAX31343_TrickleChargerMode_6k_without_diode = 0x2,
	MAX31343_TrickleChargerMode_11k_without_diode = 0x3,
	MAX31343_TrickleChargerMode_3k_with_diode = 0x4,
	MAX31343_TrickleChargerMode_6k_with_diode = 0x6,
	MAX31343_TrickleChargerMode_11k_with_diode = 0x7,
	MAX31343_TrickleChargerMode_no_connection = 0x8,
} MAX31343_TrickleChargerMode;

typedef enum {
	MAX31343_TemperatureSensor_AutomaticModeInterval_1sec = 0x0,
	MAX31343_TemperatureSensor_AutomaticModeInterval_2sec = 0x1,
	MAX31343_TemperatureSensor_AutomaticModeInterval_4sec = 0x2,
	MAX31343_TemperatureSensor_AutomaticModeInterval_8sec = 0x3,
	MAX31343_TemperatureSensor_AutomaticModeInterval_16sec = 0x4,
	MAX31343_TemperatureSensor_AutomaticModeInterval_32sec = 0x5,
	MAX31343_TemperatureSensor_AutomaticModeInterval_64sec = 0x6,
	MAX31343_TemperatureSensor_AutomaticModeInterval_128sec = 0x7,
} MAX31343_TemperatureSensor_AutomaticModeInterval;

typedef enum {
	MAX31343_AlarmType_EverySecond,
	MAX31343_AlarmType_EveryMinute,
	MAX31343_AlarmType_EveryHour,
	MAX31343_AlarmType_EveryDay,
	MAX31343_AlarmType_EveryWeek,
	MAX31343_AlarmType_EveryMonth,
	MAX31343_AlarmType_EveryYear,
	MAX31343_AlarmType_Once,
	MAX31343_AlarmType_NonStandard,
} MAX31343_AlarmType;

typedef struct MAX3134_STRUCT_PACKED {
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t date;
	uint8_t month;
	uint8_t year;
	uint8_t century;
	uint8_t dayOfWeek;

	int isBcdFormatUsed;
} MAX31343_DateTime;

#define MAX31343_I2C_7BIT_ADDRESS           0x68
#define MAX31343_I2C_READ_ADDRESS           ((MAX31343_I2C_7BIT_ADDRESS << 1) | 0x01)
#define MAX31343_I2C_WRITE_ADDRESS          ((MAX31343_I2C_7BIT_ADDRESS << 1) | 0x00)

#define MAX31343_REGISTER_STATUS            0x00
#define MAX31343_REGISTER_INT_EN            0x01
#define MAX31343_REGISTER_RTC_RESET         0x02
#define MAX31343_REGISTER_RTC_CONFIG1       0x03
#define MAX31343_REGISTER_RTC_CONFIG2       0x04
#define MAX31343_REGISTER_TIMER_CONFIG      0x05
#define MAX31343_REGISTER_SECONDS           0x06
#define MAX31343_REGISTER_MINUTES           0x07
#define MAX31343_REGISTER_HOURS             0x08
#define MAX31343_REGISTER_DAY               0x09
#define MAX31343_REGISTER_DATE              0x0A
#define MAX31343_REGISTER_MONTH             0x0B
#define MAX31343_REGISTER_YEAR              0x0C
#define MAX31343_REGISTER_ALM1_SEC          0x0D
#define MAX31343_REGISTER_ALM1_MIN          0x0E
#define MAX31343_REGISTER_ALM1_HRS          0x0F
#define MAX31343_REGISTER_ALM1DAY_DATE      0x10
#define MAX31343_REGISTER_ALM1_MON          0x11
#define MAX31343_REGISTER_ALM1_YEAR         0x12
#define MAX31343_REGISTER_ALM2_MIN          0x13
#define MAX31343_REGISTER_ALM2_HRS          0x14
#define MAX31343_REGISTER_ALM2DAY_DATE      0x15
#define MAX31343_REGISTER_TIMER_COUNT       0x16
#define MAX31343_REGISTER_TIMER_INIT        0x17
#define MAX31343_REGISTER_PWR_MGMT          0x18
#define MAX31343_REGISTER_TRICKLE_REG       0x19
#define MAX31343_REGISTER_TEMP_MSB          0x1A
#define MAX31343_REGISTER_TEMP_LSB          0x1B
#define MAX31343_REGISTER_TS_CONFIG         0x1C
#define MAX31343_REGISTER_RAM_REG			0x22

// field definition is in format registerAddress, non-shifted filedMask, shift 
// offset. This pattern matches parameters of MAX31343_WriteField and 
// MAX31343_ReadField functions.
#define MAX31343_FIELD_A1F                MAX31343_REGISTER_STATUS,       0x1,  0
#define MAX31343_FIELD_A2F                MAX31343_REGISTER_STATUS,       0x1,  1
#define MAX31343_FIELD_TIF                MAX31343_REGISTER_STATUS,       0x1,  2
#define MAX31343_FIELD_TSF                MAX31343_REGISTER_STATUS,       0x1,  3
#define MAX31343_FIELD_PFAIL              MAX31343_REGISTER_STATUS,       0x1,  5
#define MAX31343_FIELD_OSF                MAX31343_REGISTER_STATUS,       0x1,  6
#define MAX31343_FIELD_PSDECT             MAX31343_REGISTER_STATUS,       0x1,  7

#define MAX31343_FIELD_INTFLAGS           MAX31343_REGISTER_STATUS,      0x6F,  0

#define MAX31343_FIELD_A1IE               MAX31343_REGISTER_INT_EN,       0x1,  0
#define MAX31343_FIELD_A2IE               MAX31343_REGISTER_INT_EN,       0x1,  1
#define MAX31343_FIELD_TIE                MAX31343_REGISTER_INT_EN,       0x1,  2
#define MAX31343_FIELD_TSIE               MAX31343_REGISTER_INT_EN,       0x1,  3
#define MAX31343_FIELD_PFAILIE            MAX31343_REGISTER_INT_EN,       0x1,  5
#define MAX31343_FIELD_DOSF               MAX31343_REGISTER_INT_EN,       0x1,  6

#define MAX31343_FIELD_SWRST              MAX31343_REGISTER_RTC_RESET,    0x1,  0

#define MAX31343_FIELD_ENOSC              MAX31343_REGISTER_RTC_CONFIG1,  0x1,  1
#define MAX31343_FIELD_I2C_TIMEOUT        MAX31343_REGISTER_RTC_CONFIG1,  0x1,  3
#define MAX31343_FIELD_DATA_RET           MAX31343_REGISTER_RTC_CONFIG1,  0x1,  4

#define MAX31343_FIELD_SQW_HZ             MAX31343_REGISTER_RTC_CONFIG2,  0x7,  0
#define MAX31343_FIELD_CLKO_HZ            MAX31343_REGISTER_RTC_CONFIG2,  0xF,  3
#define MAX31343_FIELD_ENCLKO             MAX31343_REGISTER_RTC_CONFIG2,  0x1,  7

#define MAX31343_FIELD_MONTH              MAX31343_REGISTER_MONTH,        0x1f, 0
#define MAX31343_FIELD_CENTURY            MAX31343_REGISTER_MONTH,        0x1,  7

#define MAX31343_FIELD_A1M1               MAX31343_REGISTER_ALM1_SEC,     0x1,  7
#define MAX31343_FIELD_A1M2               MAX31343_REGISTER_ALM1_MIN,     0x1,  7
#define MAX31343_FIELD_A1M3               MAX31343_REGISTER_ALM1_HRS,     0x1,  7
#define MAX31343_FIELD_A1DY_DT            MAX31343_REGISTER_ALM1DAY_DATE, 0x1,  6
#define MAX31343_FIELD_A1M4               MAX31343_REGISTER_ALM1DAY_DATE, 0x1,  7
#define MAX31343_FIELD_A1M6               MAX31343_REGISTER_ALM1_MON,     0x1,  6
#define MAX31343_FIELD_A1M5               MAX31343_REGISTER_ALM1_MON,     0x1,  7

#define MAX31343_FIELD_A2M2               MAX31343_REGISTER_ALM2_MIN,     0x1,  7
#define MAX31343_FIELD_A2M3               MAX31343_REGISTER_ALM2_HRS,     0x1,  7
#define MAX31343_FIELD_A2DY_DT            MAX31343_REGISTER_ALM1DAY_DATE, 0x1,  6
#define MAX31343_FIELD_A2M4               MAX31343_REGISTER_ALM2DAY_DATE, 0x1,  7

#define MAX31343_FIELD_D_TRICKLE          MAX31343_REGISTER_TRICKLE_REG,  0xF,  0
#define MAX31343_FIELD_TCHE               MAX31343_REGISTER_TRICKLE_REG,  0xF,  4

#define MAX31343_FIELD_D_MAN_SEL          MAX31343_REGISTER_PWR_MGMT,     0x1,  2
#define MAX31343_FIELD_D_VBACK_SEL        MAX31343_REGISTER_PWR_MGMT,     0x1,  3
#define MAX31343_FIELD_PFVT               MAX31343_REGISTER_PWR_MGMT,     0x3,  4

#define MAX31343_FIELD_TFS                MAX31343_REGISTER_TIMER_CONFIG, 0x3,  0
#define MAX31343_FIELD_TRPT               MAX31343_REGISTER_TIMER_CONFIG, 0x1,  2
#define MAX31343_FIELD_TP                 MAX31343_REGISTER_TIMER_CONFIG, 0x1,  3
#define MAX31343_FIELD_TE                 MAX31343_REGISTER_TIMER_CONFIG, 0x1,  4

#define MAX31343_FIELD_TTSINT             MAX31343_REGISTER_TS_CONFIG,    0x7,  3
#define MAX31343_FIELD_ONESHOTMODE        MAX31343_REGISTER_TS_CONFIG,    0x1,  6
#define MAX31343_FIELD_AUTOMODE           MAX31343_REGISTER_TS_CONFIG,    0x1,  7

#define MAX31343_RAM_SIZE                 64

MAX31343_Status MAX31343_Init();
MAX31343_Status MAX31343_InitInterruptsSupport();
MAX31343_Status MAX31343_Deinit();

MAX31343_Status MAX31343_Reset();

MAX31343_Status MAX31343_ConfigureInterrupts(MAX31343_Interrupt enabledInterrupts);
MAX31343_Status MAX31343_EnableInterrupts(MAX31343_Interrupt enableInterrupts);
MAX31343_Status MAX31343_DisableInterrupts(MAX31343_Interrupt disableInterrupts);
MAX31343_Status MAX31343_GetPendingInterrups(MAX31343_Interrupt* pendingInterrupts);

MAX31343_Status MAX31343_SetDateTime(MAX31343_DateTime* datetime);
MAX31343_Status MAX31343_GetDateTime(MAX31343_DateTime* datetime, int getDateTimeInBcdFormat);
MAX31343_Status MAX31343_SetDate(MAX31343_DateTime* date);
MAX31343_Status MAX31343_GetDate(MAX31343_DateTime* date, int getDateInBcdFormat);
MAX31343_Status MAX31343_SetTime(MAX31343_DateTime* time);
MAX31343_Status MAX31343_GetTime(MAX31343_DateTime* time, int getTimeInBcdFormat);

MAX31343_Status MAX31343_DataRetention_Enable();
MAX31343_Status MAX31343_DataRetention_Disable();
MAX31343_Status MAX31343_DataRetention_IsEnabled(int* isDataRetentionEnabled);

MAX31343_Status MAX31343_Oscilator_Enable();
MAX31343_Status MAX31343_Oscilator_Disable();
MAX31343_Status MAX31343_Oscilator_IsEnabled(int* isOscilatorEnabled);

MAX31343_Status MAX31343_I2CTimeout_Enable();
MAX31343_Status MAX31343_I2CTimeout_Disable();
MAX31343_Status MAX31343_I2CTimeout_IsEnabled(int* isI2CTimeoutEnabled);

MAX31343_Status MAX31343_OutputClocks_SetSqwFrequency(MAX31343_SqwFrequency sqwFrequency);
MAX31343_Status MAX31343_OutputClocks_GetSqwFrequency(MAX31343_SqwFrequency* sqwFrequency);
MAX31343_Status MAX31343_OutputClocks_SetClkoFrequency(MAX31343_ClkoFrequency clkoFrequency);
MAX31343_Status MAX31343_OutputClocks_GetClkoFrequency(MAX31343_ClkoFrequency* clkoFrequency);
MAX31343_Status MAX31343_OutputClocks_EnableClko();
MAX31343_Status MAX31343_OutputClocks_DisableClko();
MAX31343_Status MAX31343_OutputClocks_IsClkoEnabled(int* isClkoEnabled);

MAX31343_Status MAX31343_Power_ConfigureAutomaticPowerSupplySelection(MAX31343_PowerFailTreshold powerFailTreshold);
MAX31343_Status MAX31343_Power_ConfigureManualPowerSupplySelection(int enableBackupPowerSupply);

MAX31343_Status MAX31343_Power_GetPowerSupplySelectionMode(MAX31343_PowerSupplySelectionMode* powerSupplySelectionMode);
MAX31343_Status MAX31343_Power_GetPowerFailTreshold(MAX31343_PowerFailTreshold* powerFailTreshold);
MAX31343_Status MAX31343_Power_IsBackupPowerSupplyEnabled(int* isBackupPowerSupplyEnabled);
MAX31343_Status MAX31343_Power_GetCurrentPowerSupplyUsed(MAX31343_PowerSupplyType* currentPowerSupplyUsed);

MAX31343_Status MAX31343_TricleCharger_SetMode(MAX31343_TrickleChargerMode trickleChargerMode);
MAX31343_Status MAX31343_TricleCharger_Enable();
MAX31343_Status MAX31343_TricleCharger_Disable();
MAX31343_Status MAX31343_TricleCharger_IsEnabled(int* isTrickleChargerEnabled);
MAX31343_Status MAX31343_TricleCharger_SetModeAndEnable(MAX31343_TrickleChargerMode trickleChargerMode);
MAX31343_Status MAX31343_TricleCharger_GetMode(MAX31343_TrickleChargerMode* trickleChargerMode);

MAX31343_Status MAX31343_Timer_Configure(MAX31343_Timer_RepeatMode repeatMode, MAX31343_Timer_Frequency frequency, uint8_t autoReloadValue, int startImmediately);
MAX31343_Status MAX31343_Timer_Start();
MAX31343_Status MAX31343_Timer_Stop();
MAX31343_Status MAX31343_Timer_Pause();
MAX31343_Status MAX31343_Timer_SetRepeatMode(MAX31343_Timer_RepeatMode repeatMode);
MAX31343_Status MAX31343_Timer_GetRepeatMode(MAX31343_Timer_RepeatMode* repeatMode);
MAX31343_Status MAX31343_Timer_SetFrequency(MAX31343_Timer_Frequency frequency);
MAX31343_Status MAX31343_Timer_GetFrequency(MAX31343_Timer_Frequency* frequency);
MAX31343_Status MAX31343_Timer_GetCounter(uint8_t* value);
MAX31343_Status MAX31343_Timer_SetAutoReloadValue(uint8_t value);
MAX31343_Status MAX31343_Timer_GetAutoReloadValue(uint8_t* value);

MAX31343_Status MAX31343_Alarm1_ConfigureEverySecond();
MAX31343_Status MAX31343_Alarm1_ConfigureEveryMinute(uint8_t atSecond, int isNumberInBcdFormat);
MAX31343_Status MAX31343_Alarm1_ConfigureEveryHour(uint8_t atMinute, uint8_t atSecond, int areNumbersInBcdFormat);
MAX31343_Status MAX31343_Alarm1_ConfigureEveryDay(uint8_t atHour, uint8_t atMinute, uint8_t atSecond, int areNumbersInBcdFormat);
MAX31343_Status MAX31343_Alarm1_ConfigureEveryWeek(uint8_t atDayOfWeek, uint8_t atHour, uint8_t atMinute, uint8_t atSecond, int areNumbersInBcdFormat);
MAX31343_Status MAX31343_Alarm1_ConfigureEveryMonth(uint8_t atDate, uint8_t atHour, uint8_t atMinute, uint8_t atSecond, int areNumbersInBcdFormat);
MAX31343_Status MAX31343_Alarm1_ConfigureEveryYear(uint8_t atMonth, uint8_t atDate, uint8_t atHour, uint8_t atMinute, uint8_t atSecond, int areNumbersInBcdFormat);
MAX31343_Status MAX31343_Alarm1_ConfigureOnce(uint8_t atYear, uint8_t atMonth, uint8_t atDate, uint8_t atHour, uint8_t atMinute, uint8_t atSecond, int areNumbersInBcdFormat);
MAX31343_Status MAX31343_Alarm1_GetAlarmType(MAX31343_AlarmType* alarmType);
MAX31343_Status MAX31343_Alarm1_GetConfiguration(MAX31343_AlarmType* alarmType, uint8_t* atYear, uint8_t* atMonth, uint8_t* atDate, uint8_t* atDayOfWeek, uint8_t* atHour, uint8_t* atMinute, uint8_t* atSecond, int getNumbersInBcdFormat);
MAX31343_Status MAX31343_Alarm1_GetEveryMinuteConfiguration(uint8_t* atSecond, int getNumbersInBcdFormat);
MAX31343_Status MAX31343_Alarm1_GetEveryHourConfiguration(uint8_t* atMinute, uint8_t* atSecond, int getNumbersInBcdFormat);
MAX31343_Status MAX31343_Alarm1_GetEveryDayConfiguration(uint8_t* atHour, uint8_t* atMinute, uint8_t* atSecond, int getNumbersInBcdFormat);
MAX31343_Status MAX31343_Alarm1_GetEveryWeekConfiguration(uint8_t* atDayOfWeek, uint8_t* atHour, uint8_t* atMinute, uint8_t* atSecond, int getNumbersInBcdFormat);
MAX31343_Status MAX31343_Alarm1_GetEveryMonthConfiguration(uint8_t* atDate, uint8_t* atHour, uint8_t* atMinute, uint8_t* atSecond, int getNumbersInBcdFormat);
MAX31343_Status MAX31343_Alarm1_GetEveryYearConfiguration(uint8_t* atMonth, uint8_t* atDate, uint8_t* atHour, uint8_t* atMinute, uint8_t* atSecond, int getNumbersInBcdFormat);
MAX31343_Status MAX31343_Alarm1_GetOnceConfiguration(uint8_t* atYear, uint8_t* atMonth, uint8_t* atDate, uint8_t* atHour, uint8_t* atMinute, uint8_t* atSecond, int getNumbersInBcdFormat);

MAX31343_Status MAX31343_Alarm2_ConfigureEveryMinute();
MAX31343_Status MAX31343_Alarm2_ConfigureEveryHour(uint8_t atMinute, int isNumberInBcdFormat);
MAX31343_Status MAX31343_Alarm2_ConfigureEveryDay(uint8_t atHour, uint8_t atMinute, int areNumbersInBcdFormat);
MAX31343_Status MAX31343_Alarm2_ConfigureEveryWeek(uint8_t atDayOfWeek, uint8_t atHour, uint8_t atMinute, int areNumbersInBcdFormat);
MAX31343_Status MAX31343_Alarm2_ConfigureEveryMonth(uint8_t atDate, uint8_t atHour, uint8_t atMinute, int areNumbersInBcdFormat);
MAX31343_Status MAX31343_Alarm2_GetAlarmType(MAX31343_AlarmType* alarmType);
MAX31343_Status MAX31343_Alarm2_GetConfiguration(MAX31343_AlarmType* alarmType, uint8_t* atDate, uint8_t* atDayOfWeek, uint8_t* atHour, uint8_t* atMinute, int getNumbersInBcdFormat);
MAX31343_Status MAX31343_Alarm2_GetEveryHourConfiguration(uint8_t* atMinute, int getNumbersInBcdFormat);
MAX31343_Status MAX31343_Alarm2_GetEveryDayConfiguration(uint8_t* atHour, uint8_t* atMinute, int getNumbersInBcdFormat);
MAX31343_Status MAX31343_Alarm2_GetEveryWeekConfiguration(uint8_t* atDayOfWeek, uint8_t* atHour, uint8_t* atMinute, int getNumbersInBcdFormat);
MAX31343_Status MAX31343_Alarm2_GetEveryMonthConfiguration(uint8_t* atDate, uint8_t* atHour, uint8_t* atMinute, int getNumbersInBcdFormat);

MAX31343_Status MAX31343_TemperatureSensor_GetTemperatureRaw(uint16_t* temperature);
MAX31343_Status MAX31343_TemperatureSensor_GetTemperatureFloat(float* temperature);
MAX31343_Status MAX31343_TemperatureSensor_EnableAutomaticMode();
MAX31343_Status MAX31343_TemperatureSensor_DisableAutomaticMode();
MAX31343_Status MAX31343_TemperatureSensor_IsAutomaticModeEnabled(int* isAutomaticModeEnabled);
MAX31343_Status MAX31343_TemperatureSensor_TriggerOneShotConversion();
MAX31343_Status MAX31343_TemperatureSensor_IsOneShotConversionDone(int* isOneShotConversionDone);
MAX31343_Status MAX31343_TemperatureSensor_SetAutomaticModeInterval(MAX31343_TemperatureSensor_AutomaticModeInterval interval);
MAX31343_Status MAX31343_TemperatureSensor_EnableAutomaticModeAndSetInterval(MAX31343_TemperatureSensor_AutomaticModeInterval interval);
MAX31343_Status MAX31343_TemperatureSensor_GetAutomaticModeInterval(MAX31343_TemperatureSensor_AutomaticModeInterval* interval);

MAX31343_Status MAX31343_ReadRam(uint8_t offset, uint8_t* buffer, size_t bufferSize);
MAX31343_Status MAX31343_WriteRam(uint8_t offset, uint8_t* buffer, size_t bufferSize);

#endif
