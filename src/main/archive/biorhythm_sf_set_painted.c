/* Function: func_8006BE64 */
/* Address: 0x8006BE64 */
/* Size: 1348 bytes (337 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script func_8006BE64_postproc.py */
#include "types.h"

extern s16 para_bun;
extern s16 para_ri;
extern s16 para_gei;
extern s16 para_un;
extern u16 basyo_pack;
extern s16 para_zat;
extern s16 para_you;
extern s16 para_kon;
extern u16 pass_days;
extern u16 ev_flag00[];

extern s32 func_8006C3D4(s32, s32, s32, s32);
extern s32 rand(void);

s32 func_8006BE64(s32 t7_hidden, s16 v0_in) {
    s32 t0;
    s32 v1;
    s32 t1;
    s32 t5;
    s32 t7;
    s32 randval;
    t0 = 0;

    switch (t7_hidden & 0xF) {
    case 0:
        t0 = func_8006C3D4(0, 0, 1, 16) + para_bun / 16;
        break;
    case 1:
        t0 = func_8006C3D4(0, 0, 1, 16) + para_ri / 16;
        break;
    case 2:
        t0 = func_8006C3D4(0, 1, 0, 16) + para_gei / 16;
        break;
    case 3:
        t0 = func_8006C3D4(1, 0, 0, 16) + para_un / 16;
        break;
    case 4:
        v1 = (u32)basyo_pack >> 12;
        switch (v1) {
        case 0:
            t0 = func_8006C3D4(0, 0, 1, 16);
            v1 = (u32)basyo_pack >> 12;
            break;
        case 1:
            t0 = func_8006C3D4(0, 1, 0, 16);
            v1 = (u32)basyo_pack >> 12;
            break;
        case 2:
            t0 = func_8006C3D4(0, 0, 1, 16);
            v1 = (u32)basyo_pack >> 12;
            break;
        case 3:
            t0 = func_8006C3D4(0, 0, 1, 16);
            v1 = (u32)basyo_pack >> 12;
            break;
        case 4:
            t0 = func_8006C3D4(0, 0, 1, 16);
            v1 = (u32)basyo_pack >> 12;
            break;
        case 5:
            t0 = func_8006C3D4(0, 1, 0, 16);
            v1 = (u32)basyo_pack >> 12;
            break;
        case 6:
            t0 = func_8006C3D4(0, 1, 0, 16);
            v1 = (u32)basyo_pack >> 12;
            break;
        case 7:
            t0 = func_8006C3D4(1, 0, 0, 16);
            v1 = (u32)basyo_pack >> 12;
            break;
        case 8:
            t0 = func_8006C3D4(1, 0, 0, 16);
            v1 = (u32)basyo_pack >> 12;
            break;
        case 9:
            t0 = func_8006C3D4(1, 0, 0, 16);
            v1 = (u32)basyo_pack >> 12;
            break;
        case 10:
            t0 = func_8006C3D4(1, 0, 0, 16);
            v1 = (u32)basyo_pack >> 12;
            break;
        default:
            break;
        }
        t0 += (u32)ev_flag00[v1 * 2] >> 4;
        break;
    case 5:
        t0 = func_8006C3D4(0, 1, 0, 16) + para_kon / 16;
        break;
    case 6:
        t0 = func_8006C3D4(0, 1, 0, 16) + para_you / 16;
        break;
    case 7:
        func_8006C3D4(1, 1, 1, 16);
        t0 = 16;
        break;
    default:
        break;
    }

    t0 += 5;

    {
        s32 tmp;
        tmp = para_zat;

        if (tmp < 97) {
            t1 = tmp / 32;
        } else {
            t1 = 4;
        }
    }

    randval = rand();

    t5 = t0 + 4;
    t7 = 8 - t1;
    t0 = (t7 * (randval % t5)) / 8;

    v1 = 0;
    if (func_8006C3D4(1, 0, 0, 3) == 0) {
        v1 = 1;
    }
    if (func_8006C3D4(0, 1, 0, 3) == 0) {
        v1++;
    }
    if (func_8006C3D4(0, 0, 1, 3) == 0) {
        v1++;
    }
    if (v1 == 3) {
        t0 = t0 / 4;
    }

    pass_days = (u16)v0_in;
    if (t0 < 5) {
        return 1;
    }
    return 0;
}
