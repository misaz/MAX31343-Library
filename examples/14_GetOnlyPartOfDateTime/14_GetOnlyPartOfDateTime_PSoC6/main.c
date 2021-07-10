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

    printf("========= Example 14_GetOnlyPartOfDateTime =========\r\n");

    // get time from chip in BCD format
    MAX31343_DateTime dt;
    mStatus = MAX31343_GetTime(&dt, 0); // 0 = normal (non-BCD) format
    CY_ASSERT(mStatus == MAX31343_Status_Ok);

    // print time
    printf("Time: %d:%02d:%02d\r\n", dt.hours, dt.minutes, dt.seconds);

    while (1) {
        __WFI();
    }
}

