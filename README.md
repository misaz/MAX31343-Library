# MAX31343 RTC Library
This is library for utilizing MAX31343 RTC chip using microcontroller. Library is designed as platform independent library with support for 8-bit, 16-bit and 32-bit MCUs. Library is designed for building with GCC but can be easily ported to support other compilers. As part of this library there are predefined ports for MAX32625, AVR, STM32 and PSoC 62 platforms but you can port it to your platform easily by implementing 4 or 5 functions calling I2C driver on your platform.

Library supports configuring and following features of MAX31343 chip:

- Date and Time (read and configure)
- Partialy reading of time or date for reducing I2C bus utilization
- Configure SQW output clock frequency
- Enable, disable and configure freqeuncy for CLKO clock output
- Configure both alarms with user friendly API
- Retrieve alarm configuration of both alarms
- Choose automatic and manual powering mode
- Configure power fail treshold in automatic powering mode
- Configure active power supply in manual powering mode
- Disable (or enable) data retention and stopping (or reaenabling) oscillator
- Enable or disable I2C timeout feature

# Status
Currently all features mentioned above were implemented and some of them was also tested on real hardware. Library is currently ported only to STM32 and PSoC 62 platforms. Only basic exmaples were implemented and many examples are not provided for all platforms.

# Instalation
Download ZIP folder with library for your platform in Release section and copy bundled files to your project. If you are using Makefile, cmake or other build system you must assign both .c files to compilation. If your platform is not supported, you must use generic verison and implement functions in `MAX31343_PlatformSpecific.c` according to notes in Porting section at the end of this page.

# Examples
There are 32 examples.

| Category         | Name                         | Link to example folder                       |
| ---------------- | ---------------------------- | -------------------------------------------- |
| Date and Time    | 10_GetDateTime		          | [Details](examples/10_GetDateTime)           |
| Date and Time    | 11_GetDateTimeBcd	          | [Details](examples/11_GetDateTimeBcd)        |
| Date and Time    | 12_SetDateTime		          | [Details](examples/12_SetDateTime)           |
| Date and Time    | 13_SetDateTimeBcd	          | [Details](examples/13_SetDateTimeBcd)        |
| Date and Time    | 14_GetOnlyPartOfDateTime     | [Details](examples/14_GetOnlyPartOfDateTime) |
| Output Clocks    | 20_SetSqwFrequency           | [Details](examples/20_SetSqwFrequency)       |
| Output Clocks    | 21_SetClkoFrequency          | Example not implemented yet.                 |
| Output Clocks    | 22_DisableClkoOutput         | Example not implemented yet.                 |
| Alarm 1          | 30_ConfigureAlarm1			  | [Details](examples/30_ConfigureAlarm1)       |
| Alarm 1          | 31_DisableAlarm1			  | Example not implemented yet.                 |
| Alarm 1          | 32_GetAlarm1Configuration	  | Example not implemented yet.                 |
| Alarm 2          | 40_ConfigureAlarm2			  | Example not implemented yet.                 |
| Alarm 1          | 41_DisableAlarm2			  | Example not implemented yet.                 |
| Alarm 2          | 42_GetAlarm2Configuration	  | Example not implemented yet.                 |
| Powering options | 50_ManualPoweringMode        | Example not implemented yet.                 |
| Powering options | 51_AutomaticPoweringMode     | Example not implemented yet.                 |
| Powering options | 53_GetCurrentPowerSupplyUsed | Example not implemented yet.                 |
| Trickle charger  | 60_ConfigureTrickeCharger    | Example not implemented yet.                 |
| Trickle charger  | 61_DisableTrickeCharger      | Example not implemented yet.                 |
| Timer            | 70_ConfigureOneShotTimer     | Example not implemented yet.                 |
| Timer            | 71_ConfigureRepetiveTimer    | Example not implemented yet.                 |
| Timer            | 73_PauseTimer                | Example not implemented yet.                 |
| Timer            | 74_ContinueTimer             | Example not implemented yet.                 |
| Timer            | 75_StopTimer                 | Example not implemented yet.                 |
| Timer            | 76_ChangeTimerFrequency      | Example not implemented yet.                 |
| Timer            | 77_ChangeTimerReloadValue    | Example not implemented yet.                 |
| System           | 80_EnableOscilator           | Example not implemented yet.                 |
| System           | 81_DisableOscilator          | Example not implemented yet.                 |
| System           | 82_EnableDataRetention       | Example not implemented yet.                 |
| System           | 83_DisableDataRetention      | Example not implemented yet.                 |
| System           | 84_EnableI2CTimeout          | Example not implemented yet.                 |
| System           | 85_DisableI2CTimeout         | Example not implemented yet.                 |
| Interrupts       | 90_Interrupts                | [Details](examples/90_Interrupts)            |

In README file in every example folder is described purpose of example, utilized function calls and their description. README contains platform independent code. Platform independent code do not check for function errors (consider using ASSERT or better checking mechanism), do not call any platform specific initialization (like system clocking or UART initialization) and contains printf call for printing text. Some examples was also ported to some 

# Porting
If you want to integrate library to different platform than MAX32625, AVR, STM32 and PSoC62 you must use generic version of library and implement content of 4 or 5 functions (in case when you do not need for interrupt support, you can implement only 4 functions) in `MAX31343_PlatformSpecific.c` file. They are:

- `MAX31343_PlatformSpecific_Init` this function must initialize I2C driver used on your platform for accessing I2C bus. See details in comments in template.
- `MAX31343_PlatformSpecific_Deinit` if you want to support deinitialization of I2C driver when no needed anymore, you can implement this in this function. If you do not need it, you do not need to implement this function.
- `MAX31343_PlatformSpecific_ReadRegisters` must execute I2C transaction as described in comments in template.
- `MAX31343_PlatformSpecific_WriteRegisters` must execute I2C transaction as described in comments in template.

If you want support for external interrupts from MAX31343 you can configure peripheral for external interrupts in `MAX31343_PlatformSpecific_ConfigureExternalInterrupt` function.