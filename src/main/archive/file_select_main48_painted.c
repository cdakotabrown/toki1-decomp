/* Function: file_select_main48 */
/* Address: 0x8006F2BC */
/* Size: 780 bytes (195 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_file_select_main48.py */
#include "common.h"

extern u8  screen_w[];
extern u16 basyo_pack;
extern s32 frame_counter;

extern u8  girl1_basyo;
extern u8  girl2_basyo;
extern u8  girl3_basyo;
extern u8  girl4_basyo;
extern u8  D_800EE47A;
extern u8  girl5_basyo;
extern u8  D_800EEA96;

extern s32 rand(void);

s32 file_select_main48(s8 *arg0, s8 *arg1) {
    u8 *base;
    s32 quarter;
    u8 val;
    u8 *ptr;

    *arg0 = 0;
    *arg1 = 0;

    quarter = (u32)(u16)basyo_pack >> 12;

    if (rand() % 3 != 0) {
        return 0;
    }

    base = screen_w;

    if (!((*(s32 *)(base + 512) << 30) < 0)) {
        val = girl1_basyo;
        if (quarter == (u32)val >> 4) {
            ptr = base + (quarter << 2);
            if (*(u16 *)(ptr + 1644) >= 21U) {
                girl1_basyo = val | 4;
                *arg0 = 1;
                if (ptr == base) {
                    *arg1 = (s8)((frame_counter & 1) + 2);
                    return 1;
                }
                *arg1 = 4;
                return 1;
            }
        }
    }

    if (!((*(s32 *)(base + 568) << 30) < 0)) {
        val = girl2_basyo;
        if (quarter == (u32)val >> 4) {
            ptr = base + (quarter << 2);
            if (*(u16 *)(ptr + 1644) >= 21U) {
                girl2_basyo = val | 4;
                *arg0 = 2;
                if (ptr == base + 8) {
                    *arg1 = 4;
                    return 1;
                }
                *arg1 = 5;
                return 1;
            }
        }
    }

    if (!((*(s32 *)(base + 624) << 30) < 0)) {
        val = girl3_basyo;
        if (quarter == (u32)val >> 4) {
            ptr = base + (quarter << 2);
            if (*(u16 *)(ptr + 1644) >= 21U) {
                girl3_basyo = val | 4;
                *arg0 = 3;
                if (ptr == base + 16) {
                    *arg1 = 1;
                    return 1;
                }
                *arg1 = 2;
                return 1;
            }
        }
    }

    if (!((*(s32 *)(base + 680) << 30) < 0)) {
        val = girl4_basyo;
        if (quarter == (u32)val >> 4) {
            ptr = base + (quarter << 2);
            if (*(u16 *)(ptr + 1644) >= 21U) {
                girl4_basyo = val | 4;
                *arg0 = 4;
                if (ptr == base + 24) {
                    *arg1 = 1;
                    return 1;
                }
                *arg1 = 2;
                return 1;
            }
        }
    }

    if (!((*(s32 *)(base + 736) << 30) < 0)) {
        if (quarter == 8) {
            if ((u32)D_800EE47A >> 4 != 8) {
                ptr = base + (quarter << 2);
                if (*(u16 *)(ptr + 1644) >= 21U) {
                    girl5_basyo = girl5_basyo | 4;
                    *arg0 = 5;
                    *arg1 = 1;
                    return 1;
                }
            }
        }
    }

    if (!((*(s32 *)(base + 792) << 30) < 0)) {
        if (quarter == 9) {
            if ((u32)D_800EE47A >> 4 != 9) {
                D_800EEA96 = D_800EEA96 | 4;
                *arg0 = 6;
                *arg1 = 1;
                return 1;
            }
        }
    }

    return 0;
}
