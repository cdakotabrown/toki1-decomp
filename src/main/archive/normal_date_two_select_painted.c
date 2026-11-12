/* Function: normal_date_two_select */
/* Address: 0x800915A8 */
/* Size: 1076 bytes (269 instructions) */
/* Region: Library (0x80086000+) */
/* @CompileFlags: -Wo,-no_const_in_reg */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_normal_date_two_select.py */
#include "types.h"

typedef struct {
    s32 field0;
    u8 *field4;
    u8 *field8;
    u8 *field12;
    u8 field16;
    u8 field17;
    u8 field18;
    u8 field19;
    u8 field20;
    u8 field21;
    u8 field22;
    u8 field23[20];
    u8 field43;
    u8 field44[16];
    u8 pad60[10];
    s16 field70;
    s16 field72;
    s16 field74;
    s16 field76[16];
    s16 field108;
    s16 field110;
    s16 field112;
    u8 pad114[4];
    s16 field118;
    s16 field120;
    u8 pad122[2];
    s32 field124;
    s32 field128;
    s32 field132;
    s32 field136;
    s32 field140;
} SelectStruct;

extern s32 ReadDeltaValue(s16, s16);
extern s32 VBLANK_MINUS;

s32 normal_date_two_select(SelectStruct *s, s32 flag, s32 arg2, u8 *data) {
    s32 i;
    u8 hi_byte, lo_byte;
    u8 b2, b3;
    s32 val;
    s32 a2_acc;
    s32 result;
    s32 rem;
    s32 half;
    s32 sr;
    s32 prod;
    s32 prod10;
    s32 sr60;
    s32 tmp2;

    s->field72 = 0;
    s->field19 = 0;
    s->field20 = 0;
    s->field23[18] = 0;
    s->field21 = 0;
    s->field22 = 0;
    s->field23[19] = 0;
    s->field18 = 0;
    s->field124 = 0;
    s->field128 = 0;
    s->field132 = 0;
    s->field112 = 0;
    s->field70 = 0;
    s->field43 = 0;
    s->field136 = 0;
    s->field23[16] = 0;
    s->field23[17] = 0;
    s->field16 = 0;
    s->field17 = 0;
    arg2 = (s16)arg2;
    s->field74 = arg2;

    for (i = 0; i < 16; i++) {
        s->field44[i] = (u8)i;
        s->field23[i] = 64;
        s->field76[i] = 127;
    }

    s->field4 = data;

    s->field108 = 1;
    s->field118 = 127;
    s->field120 = 127;

    if (flag != 0) {
        a2_acc = 2;
        s->field4 += 2;
    } else {
        a2_acc = 8;
        s->field4 += 8;
    }

    hi_byte = *s->field4;
    s->field4++;
    lo_byte = *s->field4;
    s->field4++;
    s->field72 = (s16)((s16)hi_byte << 8) | lo_byte;

    b2 = *s->field4;
    s->field4++;
    b3 = *s->field4;
    s->field4++;
    hi_byte = *s->field4;
    s->field4++;
    lo_byte = *s->field4;
    s->field4++;

    val = (b2 << 16) | (b3 << 8) | lo_byte;

    s->field132 = val;
    a2_acc += 2;
    a2_acc += 3;

    rem = 0x03938700 % s->field132;
    half = s->field132;
    if (half < 0) {
        half = half + 1;
    }
    half >>= 1;
    if (half < rem) {
        s->field132 = 0x03938700 / s->field132 + 1;
    } else {
        s->field132 = 0x03938700 / s->field132;
    }

    s->field4 += 2;
    s->field140 = s->field132;

    hi_byte = *s->field4;
    s->field4++;
    lo_byte = *s->field4;
    s->field4++;
    a2_acc += 2;
    b2 = *s->field4;
    s->field4++;
    a2_acc += 4;
    b3 = *s->field4;
    s->field4++;

    result = ReadDeltaValue(arg2, flag);

    s->field124 = result;
    s->field136 = result;
    s->field8 = s->field4;
    s->field12 = s->field4;

    sr = VBLANK_MINUS;
    prod = (s32)s->field72 * s->field132;
    prod10 = prod * 10;

    if ((u32)prod10 < (u32)(sr * 60)) {

        s->field108 = (s16)((u32)(sr * 600) / (u32)prod);
        s->field110 = s->field108;
    } else {

        s->field108 = -1;
        sr60 = VBLANK_MINUS * 60;
        s->field110 = (s16)((u32)sr60 / (u32)prod10);
        tmp2 = VBLANK_MINUS * 60;
        if ((u32)tmp2 % (u32)sr60 > (u32)tmp2 / 2) {
            s->field110 = s->field110 + 1;
        }
    }

    s->field112 = s->field110;

    return a2_acc + (hi_byte << 24) + (lo_byte << 16) + (b2 << 8) + b3;
}
