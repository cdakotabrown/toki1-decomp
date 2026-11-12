/* Function: mode_change_check */
/* Address: 0x8006EC08 */
/* Size: 864 bytes (216 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* UNMATCHABLE FROM C: */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_func_8006EC08.py */
/* @PostProc: custom_script custom_func_8006EC08.py */
#include "types.h"

extern s32 rand(void);

extern s32 girl1_flag;
extern s32 girl2_flag;
extern s32 girl3_flag;
extern s32 girl5_flag;
extern s32 girl6_flag;
extern s32 girl7_flag;
extern s32 girl8_flag;
extern s32 girl4_flag;

extern s32 sched_icon;
extern s32 frame_counter;

extern s16 para_bun;
extern s16 para_ri;
extern s16 para_gei;
extern s16 para_un;
extern s16 para_zat;
extern s16 para_you;
extern s16 para_kon;

extern u8 girl0_basyo;
extern u16 basyo_pack;
extern u8 girl4_basyo;

s32 mode_change_check(s8 *arg0, s8 *arg1) {
    s32 a3;
    s32 t7;
    s32 bits;

    *arg0 = 0;
    *arg1 = 0;
    if (rand() % 3 != 0) {
        return 0;
    }

    if (!((girl1_flag << 30) < 0)) {
        a3 = sched_icon;
        if (!(a3 & 0xF) && para_bun >= 0x37) {
            *arg0 = 1;
            *arg1 = frame_counter & 1;
            return 1;
        }
    }

    if (!((girl2_flag << 30) < 0)) {
        a3 = sched_icon;
        if ((a3 & 0xF) == 1 && para_ri >= 0x3C) {
            *arg0 = 2;
            *arg1 = frame_counter & 3;
            return 1;
        }
    }

    a3 = sched_icon;
    if (!((girl3_flag << 30) < 0) && (a3 & 0xF) == 2 && para_gei >= 0x32) {
        *arg0 = 3;
        *arg1 = 0;
        return 1;
    }

    if (!((girl5_flag << 30) < 0) && (a3 & 0xF) == 3 &&
        ((u8)girl0_basyo >> 4) != 8 &&
        para_un >= 0x51 && para_you >= 0x51) {
        *arg0 = 5;
        *arg1 = 0;
        return 1;
    }

    if (!((girl6_flag << 30) < 0) && (a3 & 0xF) == 3 &&
        ((u8)girl0_basyo >> 4) != 9 &&
        para_un >= 0x1F && para_kon >= 0x1F) {
        *arg0 = 6;
        *arg1 = 0;
        return 1;
    }

    if (!((girl7_flag << 30) < 0) && (a3 & 0xF) == 6 && para_you >= 0x65) {
        *arg0 = 7;
        *arg1 = 0;
        return 1;
    }

    t7 = girl8_flag;
    if (!((t7 << 30) < 0)) {
        bits = (t7 << 0x14) >> 0x1d;
        if (bits == 0 && para_zat >= 0x24) {
            *arg0 = 8;
            *arg1 = 0;
            return 1;
        }
        if (bits == 1 && para_zat >= 0x38) {
            *arg0 = 8;
            *arg1 = 1;
            return 1;
        }
        if (bits == 2 && para_zat >= 0x4C) {
            *arg0 = 8;
            *arg1 = 2;
            return 1;
        }
        if (bits == 3 && (a3 & 0xF) == 5 && para_zat >= 0x5B) {
            *arg0 = 8;
            *arg1 = 3;
            return 1;
        }
    }

    if (!((girl4_flag << 30) < 0) &&
        ((u32)basyo_pack >> 0xC) != ((u8)girl4_basyo >> 4) &&
        para_kon >= 0x24) {
        *arg0 = 4;
        *arg1 = 0;
        return 1;
    }

    return 0;
}
