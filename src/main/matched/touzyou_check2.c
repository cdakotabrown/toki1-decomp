/* Function: touzyou_check2 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=6:t9 IDO52_FORM_SPLIT=const:1:38 IDO52_EMIT_SITE=const:1:37:2:0 */
/* Address: 0x8006EF68 */
/* Size: 852 bytes (213 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_insn_reg_field 0x40:rd:t8:t9 0x44:rs:t8:t9 0x48:rt:t7:t8 0x50:rt:t4:t5 0x54:rt:t9:t4 0x58:rs:t9:t4 0x60:rs:t4:t5 0x60:rt:t4:t5 0x64:rt:t9:t2 0x68:rs:t4:t5 0x70:rt:t5:t6 0x74:rt:t9:t2 0x78:rs:t5:t6 0x78:rt:t5:t6 0x80:rs:t5:t6 0x80:rt:t6:t7 0x88:rt:t6:t7 0x8c:rs:t7:t8 0x8c:rt:t7:t8 0x98:rt:t7:t8 0x98:rd:t8:t4 0x9c:rs:t8:t4 0xa4:rt:t4:t5 0xa8:rt:t4:t5 0xac:rt:t9:t6 0xb0:rs:t9:t6 0xb0:rt:t9:t6 0xb8:rs:t9:t6 0xc0:rt:t5:t7 0xc8:rs:t5:t7 0xc8:rt:t5:t7 0xd0:rs:t5:t7 0xd0:rt:t6:t8 0xd8:rt:t6:t8 0xdc:rt:t7:t9 0xe0:rs:t7:t9 0xe0:rt:t7:t9 0xe8:rt:t7:t9 0xe8:rd:t8:t5 0xec:rs:t8:t5 0xf4:rt:t4:t6 0xf8:rt:t4:t6 0xfc:rt:t9:t7 0x100:rs:t9:t7 0x100:rt:t9:t7 0x108:rs:t9:t7 0x120:rt:t5:t8 0x124:rs:t5:t8 0x124:rt:t5:t8 0x12c:rt:t5:t8 0x12c:rd:t6:t4 0x130:rs:t6:t4 0x138:rt:t7:t5 0x13c:rt:t7:t5 0x140:rt:t8:t6 0x144:rs:t8:t6 0x144:rt:t8:t6 0x14c:rt:t8:t6 0x14c:rd:t4:t7 0x150:rt:t4:t7 0x154:rt:t9:t8 0x158:rs:t9:t8 0x158:rt:t9:t8 0x15c:rt:t5:t9 0x160:rs:t9:t8 0x16c:rs:t5:t9 0x16c:rt:t5:t9 0x170:rt:t6:t4 0x174:rs:t5:t9 0x180:rt:t6:t4 0x18c:rt:t7:t5 0x190:rs:t7:t5 0x190:rt:t7:t5 0x198:rt:t7:t5 0x198:rd:t8:t7 0x19c:rs:t8:t7 0x1a4:rt:t4:t8 0x1a8:rt:t4:t8 0x1b8:rd:t5:t4 0x1bc:rs:t5:t4 0x1c0:rt:t6:t5 0x1c4:rs:t6:t5 0x1c4:rt:t6:t5 0x1c8:rt:t7:t6 0x1cc:rs:t6:t5 0x1d8:rs:t7:t6 0x1d8:rt:t7:t6 0x1dc:rt:t8:t7 0x1e0:rs:t7:t6 0x1ec:rt:t8:t7 0x1f8:rt:t4:t8 0x1fc:rs:t4:t8 0x1fc:rt:t4:t8 0x204:rt:t4:t8 0x204:rd:t9:t4 0x208:rs:t9:t4 0x240:rt:t5:t6 0x244:rd:t8:t9 0x248:rs:t8:t9 0x28c:rs:v0:t2 0x28c:rt:t2:v0 0x2ac:rs:v0:t3 0x2ac:rt:t3:v0 0x2cc:rs:v0:t1 0x2cc:rt:t1:v0 0x2d0:rt:t9:t5 0x2d8:rs:t9:t5 0x2f4:rs:t5:t6 0x2f4:rt:t5:t6 0x2f8:rt:t7:t9 0x2fc:rt:t5:t6 0x2fc:rd:t6:t8 0x300:rs:t6:t8 0x304:rt:t4:t5 0x308:rs:t7:t9 0x308:rt:t7:t9 0x30c:rs:t4:t5 0x30c:rt:t4:t5 0x310:rt:t7:t9 0x310:rd:t8:t4 0x314:rt:t4:t5 0x314:rd:t9:t6 0x318:rs:t8:t4 0x318:rt:t9:t6 0x31c:rt:t5:t7 0x320:rs:t5:t7 0x320:rt:t5:t7 0x324:rt:t6:t8 0x328:rs:t5:t7 0x334:rt:t6:t8 */
/* @CompileFlags: -Wc,-pic1 */
/* 2026-08-23: `patch_stack_complete` retired for `@CompileFlags: -Wc,-pic1`. */
/* ⭐ CLOSER: `@CompileEnv: IDO52_EMIT_SITE=const:1:37:2:0` (Hook 30). */
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

s32 touzyou_check2(s8 *arg0, s8 *arg1) {
    s32 a3;
    s32 t7;
    s32 bits;

    if (rand() & 7) {
        return 0;
    }

    *arg0 = 0;
    *arg1 = 0;

    if (!((girl1_flag << 30) < 0)) {
        a3 = sched_icon;
        if (!(a3 & 0xF) && para_bun >= 0x4B) {
            *arg0 = 1;
            *arg1 = frame_counter & 1;
            return 1;
        }
    }

    if (!((girl2_flag << 30) < 0)) {
        a3 = sched_icon;
        if ((a3 & 0xF) == 1 && para_ri >= 0x55) {
            *arg0 = 2;
            *arg1 = frame_counter & 3;
            return 1;
        }
    }

    a3 = sched_icon;
    if (!((girl3_flag << 30) < 0) && (a3 & 0xF) == 2 && para_gei >= 0x41) {
        *arg0 = 3;
        *arg1 = 0;
        return 1;
    }

    if (!((girl5_flag << 30) < 0) && (a3 & 0xF) == 3 &&
        ((s32)((u32)girl0_basyo >> 4)) != 8 &&
        para_un >= 0x54 && para_you >= 0x54) {
        *arg0 = 5;
        *arg1 = 0;
        return 1;
    }

    if (!((girl6_flag << 30) < 0) && (a3 & 0xF) == 3 &&
        ((s32)((u32)girl0_basyo >> 4)) != 9 &&
        para_un >= 0x47 && para_kon >= 0x47) {
        *arg0 = 6;
        *arg1 = 0;
        return 1;
    }

    if (!((girl7_flag << 30) < 0) && (a3 & 0xF) == 6 && para_you >= 0x7E) {
        *arg0 = 7;
        *arg1 = 0;
        return 1;
    }

    t7 = girl8_flag;
    if (!((t7 << 30) < 0)) {
        bits = (s32)((u32)(t7 << 0x14) >> 0x1d);
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
        ((s32)((u32)girl4_basyo >> 4)) != ((s32)((u32)basyo_pack >> 0xC)) &&
        para_kon >= 0x60) {
        *arg0 = 4;
        *arg1 = 0;
        return 1;
    }

    return 0;
}
