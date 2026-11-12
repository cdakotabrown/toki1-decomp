/* Function: machiawase_set */
/* Address: 0x80076E2C */
/* Size: 288 bytes (72 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */

/* `@PostProc: custom_script custom_func_80076E2C.py` (retired/deleted; it */



















































#include "types.h"

extern u32 reserved_date;
extern u8 machiawase_basyo;
extern u8 machiawase_basyo2;
extern u8 machiawase_basyo3;
extern volatile u8 schedule_cursor;
extern u8 now_girl;
extern u8 girl_no;
extern u8 machiawase_flag;
extern u8 screen_w[2048];
extern void goto_step(s32);

void machiawase_set(void) {
    s32 pad[3];
    s32 temp[3];
    s32 *ptr;
    s32 *end;
    s32 i;
    u8 t6, t7, t8;
    u8 *base;
    s32 a0val;

    t6 = machiawase_basyo;
    t7 = machiawase_basyo2;
    t8 = machiawase_basyo3;
    temp[0] = t6;
    temp[1] = t7;
    temp[2] = t8;
    machiawase_basyo = reserved_date & 0x1F;
    base = screen_w;

#line 100
    ptr = &temp[0];
#line 101
    end = &temp[3];
#line 102
    i = 2;
#line 99
    do {
#line 105
        a0val = *ptr;
        ptr++;
        if (a0val != (s32)(*(volatile u32 *)(base + 1356) & 0x1Fu)) {
            *(base + 1828 + i) = a0val;
            i++;
        }
    } while (ptr != end);

    *(base + 1692 + schedule_cursor * 4) = 128;
    *(base + 1693 + schedule_cursor * 4) = (u8)((reserved_date << 3) >> 28);
    *(base + 1694 + schedule_cursor * 4) = reserved_date & 0x1F;

    now_girl = (reserved_date << 3) >> 28;
    girl_no = (reserved_date << 3) >> 28;
    machiawase_flag = *(volatile u8 *)(base + 1359) & 1;

    goto_step(80);
}
