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

    // read current frequency
    MAX31343_SqwFrequency current;
    mStatus = MAX31343_OutputClocks_GetSqwFrequency(&current);
    CY_ASSERT(mStatus == MAX31343_Status_Ok);

    printf("Current SQW signal clock frequency: %d Hz \n", (1 << currentú));

    // toggle frequency between 2 Hz and 4 Hz to demonstrate change
    if (current == MAX31343_SqwFrequency_2Hz) {
        mStatus = MAX31343_OutputClocks_SetSqwFrequency(MAX31343_SqwFrequency_4Hz);
        CY_ASSERT(mStatus == MAX31343_Status_Ok);

        printf("New SQW signal clock frequency: 4 Hz \n");
    }
    else {
        mStatus = MAX31343_OutputClocks_SetSqwFrequency(MAX31343_SqwFrequency_2Hz);
        CY_ASSERT(mStatus == MAX31343_Status_Ok);

        printf("New SQW signal clock frequency: 2 Hz \n");
    }

    while (1) {
        __WFI();
    }
}

