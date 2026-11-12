/* Address: 0x80056DF0 */
/* Size: 116 bytes (29 instructions) */
/* Compiler: GCC-like (0x80040000-0x80080000 region) */
#include "types.h"

extern void card_state_clear(void);
extern void card_w_clear(void);
extern void card_step_init(void);

extern u8 album_status;
extern u8 album_status1;
extern u8 D_800F01F6;
extern u8 D_800F01F7[12];
extern u8 card_sys_present;

void func_80056DF0(void) {
    volatile u8 *ptr;
    volatile u8 *end;

    card_state_clear();
    card_w_clear();

    album_status = 0;
    album_status1 = 0;
    D_800F01F6 = 0;

    end = (volatile u8 *)0x800F01FF;
    ptr = (volatile u8 *)0x800F01F3;
    do {
        ptr += 4;
        ptr[1] = 0;
        ptr[2] = 0;
        ptr[3] = 0;
        ptr[0] = 0;
    } while (ptr != end);

    card_step_init();

    card_sys_present = 0;
}
