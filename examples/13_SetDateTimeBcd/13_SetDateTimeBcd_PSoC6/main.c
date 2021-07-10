#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "MAX31343.h"

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

    printf("========= Example 13_SetDateTimeBcd =========\r\n");

    MAX31343_DateTime dt;

    // set centruy to 1 when years 2100 - 2199 are used.
    dt.century = 0;
    dt.year = 19;

    dt.month = 5;
    dt.date = 8;
    dt.dayOfWeek = 4;
    dt.hours = 12;
    dt.minutes = 34;
    dt.seconds = 56;

    // use native integers
    dt.isBcdFormatUsed = 0;

    // set RTC chip date and time 
    mStatus = MAX31343_SetDateTime(&dt);
    CY_ASSERT(mStatus == MAX31343_Status_Ok);

    printf("Time set successfully.\r\n");

    while (1) {
    	__WFI();
    }
}
