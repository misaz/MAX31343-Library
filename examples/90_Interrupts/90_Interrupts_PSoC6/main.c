#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "MAX31343.h"

int max31343InterruptFlag = 0;

void MAX31343_InterruptCallback(void *callback_arg, cyhal_gpio_event_t event) {
	max31343InterruptFlag = 1;
}

int main(void) {
	cy_rslt_t cStatus;
	MAX31343_Status mStatus;

	// platform specific init
	cStatus = cybsp_init();
	CY_ASSERT(cStatus == CY_RSLT_SUCCESS);

	// printf to UART init
	cStatus = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, 115200);
	CY_ASSERT(cStatus == CY_RSLT_SUCCESS);

	// initialize MAX31343 library
	mStatus = MAX31343_Init();
	CY_ASSERT(mStatus == MAX31343_Status_Ok);

	// enable interrupt reception on this MCU (we will enable interrupt generation by RTC chip later)
	mStatus = MAX31343_InitInterruptsSupport();
	CY_ASSERT(mStatus == MAX31343_Status_Ok);

	printf("========= Example 90_Interrupts =========\r\n");

	// configure alarm 1 to trigger every minute at 42 seconds.
	// alarm triggers for example at: 12:54:42, then at 12:55:42, then at 12:56:42, ...
	mStatus = MAX31343_Alarm1_ConfigureEveryMinute(42, 0);
	CY_ASSERT(mStatus == MAX31343_Status_Ok);

	// enable generating interrupt
	mStatus = MAX31343_EnableInterrupts(MAX31343_Interrupt_Alarm1);
	CY_ASSERT(mStatus == MAX31343_Status_Ok);

	while (1) {
		if (max31343InterruptFlag) {
			max31343InterruptFlag = 0;

			MAX31343_Interrupt flags;

			mStatus = MAX31343_GetPendingInterrups(&flags);
			CY_ASSERT(mStatus == MAX31343_Status_Ok);

			// check if alarm triggered interrupt
			if (flags & MAX31343_Interrupt_Alarm1) {
				// get date and time from chip in normal format
				MAX31343_DateTime dt;
				mStatus = MAX31343_GetDateTime(&dt, 0); // 0 = normal (non-BCD) format
				CY_ASSERT(mStatus == MAX31343_Status_Ok);

				// print date and time
				printf("Alarm triggered at: %d-%02d-%02d %d:%02d:%02d\r\n", 2000 + dt.year, dt.month, dt.date, dt.hours, dt.minutes, dt.seconds);
			}
		}
		__WFI();
	}
}
