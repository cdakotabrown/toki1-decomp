/* @CompileEnv: IDO52_UGEN_TEMP_REG=4:a1 IDO52_UGEN_TEMP_REG_INPLACE=1 */
/* @PostProc: swap_instructions --offset1 0x11c --offset2 0x120 */
/* tt_row_init - TT overlay, Address: 0x80132198, Size: 372 bytes (93 insns) */
#include "types.h"

extern u8 *tt_game_wk;
extern u8 *tt_text_buf;

extern void tt_time_set(u16 a0);
extern void tt_name_set(u8 *a0, s32 a1);

void tt_row_init(u8 arg0, u8 arg1, u8 arg2)
{
    u8 *p;
    u8 *o;
    u8 *q;
    u8 c;
    s32 i;
    s32 v;

    p = tt_game_wk;
    o = tt_text_buf;
    i = 0;
    v = 120;
    q = p + 50;
    for (; i != 3; i++) {
        switch (i) {
        case 0:
            c = arg0;
            break;
        case 1:
            c = arg1;
            break;
        case 2:
            c = arg2;
            break;
        }
        *(s16 *)(o + 10) = -76;
        *(s16 *)(o + 12) = v;
        *(s16 *)(o + 14) = i + 14;
        o[2] = c;
        o[0] = 1;
        *(s16 *)(o + 6) = 8;
        *(s16 *)(o + 16) = 0;
        *(s16 *)(o + 30) = -28;
        *(s16 *)(o + 32) = v;
        *(s16 *)(o + 34) = i + 40;
        o[22] = c;
        o[20] = 1;
        *(s16 *)(o + 26) = 8;
        *(s16 *)(o + 36) = 0;
        o += 40;
        tt_time_set(*(u16 *)(p + 48));
        tt_name_set(q, i + 43);
        *(s16 *)(o + 12) = v;
        v += 16;
        p += 6;
        q += 6;
        *(s16 *)(o + 10) = 52;
        *(s16 *)(o + 14) = i + 43;
        o[2] = c;
        o[0] = 1;
        *(s16 *)(o + 6) = 8;
        *(s16 *)(o + 16) = 0;
        o += 20;
    }
}
