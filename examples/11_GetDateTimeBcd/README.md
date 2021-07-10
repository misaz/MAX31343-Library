# 10_GetDateTime

Goal of this demo is to show you:

1) how to initialize library
2) read actual date and time from MAX31343 RTC chip

This demo show how to read time in BCD format. There are also another demo (10_GetDateTime) which shows how to get time as normal integers. BCD format may be usefull in systems with significantly constrained resources limits.

Example is implemented for PSoC62 platform in `main_psoc6.c` file. Platform independent demo is available and described later on this page.

Platform independent code of demo is following. For porting you will need to add platform specific initializations. Example use printf function which you need to provide or implement yourself.

```
#include "MAX31343.h"

int main(void) {
    MAX31343_Status mStatus;

	// initialize MAX31343 library
    mStatus = MAX31343_Init();
    // check for errors

	// get date and time from chip in normal format
    MAX31343_DateTime dt;
    mStatus = MAX31343_GetDateTime(&dt, 1); // 1 = BCD format
    CY_ASSERT(mStatus == MAX31343_Status_Ok);

	// print date and time
    printf("Date: 20%02x-%02x-%02x\r\n", dt.year, dt.month, dt.date);
    printf("Time: %02x:%02x:%02x\r\n", dt.hours, dt.minutes, dt.seconds);

    while (1) {
    }
}
```