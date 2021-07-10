#include "main.h"
#include "stm32l5xx_hal.h"
#include "MAX31343.h"
#include <string.h>
#include <stdio.h>
#include "cmsis_gcc.h"

UART_HandleTypeDef uart;

void SystemClock_Config(void);
static int UART_Init(void);
static int UART_PrintString(char* string);
static int UART_PrintFormattedInteger(char* format, int number);

#define ASSERT(status) do { if (!(status)) { __BKPT(42); } } while (0)

int main(void) {
	MAX31343_Status mStatus;
	int uStatus;

	// platform specific init
	HAL_Init();
	SystemClock_Config();

	// UART init
	uStatus = UART_Init();
	ASSERT(uStatus == 0);

	// initialize MAX31343 library
	mStatus = MAX31343_Init();
	ASSERT(mStatus == MAX31343_Status_Ok);

	uStatus = UART_PrintString("========= Example 20_SetSqwFrequency =========\r\n");
	ASSERT(uStatus == 0);

    // read current frequency
    MAX31343_SqwFrequency current;
    mStatus = MAX31343_OutputClocks_GetSqwFrequency(&current);
    ASSERT(mStatus == MAX31343_Status_Ok);

    UART_PrintString("Current SQW signal clock frequency: ");
    UART_PrintFormattedInteger("%d", (1 << current));
    UART_PrintString(" Hz \r\n");

	// toggle frequency between 2 Hz and 4 Hz to demonstrate change
	if (current == MAX31343_SqwFrequency_2Hz) {
        mStatus = MAX31343_OutputClocks_SetSqwFrequency(MAX31343_SqwFrequency_4Hz);
        ASSERT(mStatus == MAX31343_Status_Ok);

        UART_PrintString("New SQW signal clock frequency: 4 Hz \r\n");
    }
    else {
        mStatus = MAX31343_OutputClocks_SetSqwFrequency(MAX31343_SqwFrequency_2Hz);
        ASSERT(mStatus == MAX31343_Status_Ok);

        UART_PrintString("New SQW signal clock frequency: 2 Hz \r\n");
    }

	while (1) {
		__WFI();
	}
}

/**
 * Generated by STM32CubeIDE
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE0) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 55;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		Error_Handler();
	}
}

static int UART_Init(void) {
	HAL_StatusTypeDef hStatus;

	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_LPUART1_CLK_ENABLE();
	HAL_PWREx_EnableVddIO2();

	GPIO_InitTypeDef gpio;
	gpio.Pin = LPUART1_TX_Pin | LPUART1_RX_Pin;
	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	gpio.Alternate = GPIO_AF8_LPUART1;

	HAL_GPIO_Init(GPIOG, &gpio);

	uart.Instance = LPUART1;
	uart.Init.BaudRate = 115200;
	uart.Init.WordLength = UART_WORDLENGTH_8B;
	uart.Init.StopBits = UART_STOPBITS_1;
	uart.Init.Parity = UART_PARITY_NONE;
	uart.Init.Mode = UART_MODE_TX;
	uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	uart.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	uart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	uart.FifoMode = UART_FIFOMODE_DISABLE;

	hStatus = HAL_UART_Init(&uart);
	if (hStatus != HAL_OK) {
		return -1;
	}

	hStatus = HAL_UARTEx_DisableFifoMode(&uart);
	if (hStatus != HAL_OK) {
		return -1;
	}

	return 0;
}

static int UART_PrintString(char* string) {
	HAL_StatusTypeDef hStatus;

	size_t len = strlen(string);
	hStatus = HAL_UART_Transmit(&uart, (unsigned char*)string, len, 1000);
	if (hStatus) {
		return -1;
	}

	return 0;
}

static int UART_PrintFormattedInteger(char* format, int number) {
	char buffer[32];
	int retval;

	retval = snprintf(buffer, sizeof(buffer), format, number);
	if (retval < 0 || retval >= sizeof(buffer)) {
		return -1;
	}

	return UART_PrintString(buffer);
}

/**
 * Error handler used by some HAL code
 */
void Error_Handler(void) {
	__disable_irq();
	while (1) {
		__WFI();
	}
}
