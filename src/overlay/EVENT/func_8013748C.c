/* Address: 0x8013748C */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_800469F4(u32);
extern void sub_80011DFC(void);

void func_8013748C(void) {
    u32 temp_a0;

    temp_a0 = 17101;

    sub_800469F4(temp_a0);

    sub_80011DFC();
}
