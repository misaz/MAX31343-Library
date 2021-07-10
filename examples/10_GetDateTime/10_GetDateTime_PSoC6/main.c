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

    printf("========= Example 10_GetDateTime =========\r\n");

	// get date and time from chip in normal format
    MAX31343_DateTime dt;
    mStatus = MAX31343_GetDateTime(&dt, 0); // 0 = normal (non-BCD) format
    CY_ASSERT(mStatus == MAX31343_Status_Ok);

	// print date and time
    printf("Date: %d-%02d-%02d\r\n", 2000 + dt.year, dt.month, dt.date);
    printf("Time: %d:%02d:%02d\r\n", dt.hours, dt.minutes, dt.seconds);

    while (1) {
    	__WFI();
    }
}
