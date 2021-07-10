#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l5xx_hal.h"

void Error_Handler(void);

#define LPUART1_TX_Pin GPIO_PIN_7
#define LPUART1_TX_GPIO_Port GPIOG
#define LPUART1_RX_Pin GPIO_PIN_8
#define LPUART1_RX_GPIO_Port GPIOG

#ifdef __cplusplus
}
#endif

#endif
