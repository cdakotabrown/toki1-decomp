/* @SymbolStart: 0x80145EFC */
/* @SymbolSize: 996 */
/* @RodataBase: 0x80154C3C */
/* @PostProc: patch_insn_reg_field 0xe0:rt:t6:v0 0xf4:rs:t6:v0 0xf4:rt:t6:v0 0xf8:rs:t6:v0 0xf8:rt:t7:t6 0xfc:rt:t9:t8 0x100:rd:t8:t7 0x100:rt:t7:t6 0x104:rt:t8:t7 0x10c:rs:t9:t8 0x10c:rt:t9:t8 0x114:rd:t0:t9 0x114:rt:t9:t8 0x118:rd:t1:t0 0x118:rt:t0:t9 0x11c:rt:t1:t0 0x144:rt:t2:t1 0x148:rs:t2:t1 0x148:rt:t2:t1 0x14c:rt:v0:v1 0x150:rd:t3:t2 0x150:rt:t2:t1 0x154:rs:v0:v1 0x154:rt:t3:t2 0x158:rt:t4:v0 0x16c:rs:t4:v0 0x16c:rt:t4:v0 0x170:rs:t4:v0 0x170:rt:t5:t3 0x174:rt:t7:t5 0x178:rd:t6:t4 0x178:rt:t5:t3 0x17c:rs:v0:v1 0x17c:rt:t6:t4 0x184:rs:t7:t5 0x184:rt:t7:t5 0x18c:rd:t8:t6 0x18c:rt:t7:t5 0x190:rd:t9:t7 0x190:rt:t8:t6 0x194:rt:t9:t7 0x1bc:rt:t0:t8 0x1c0:rs:t0:t8 0x1c0:rt:t0:t8 0x1c8:rd:t1:t9 0x1c8:rt:t0:t8 0x1cc:rt:t1:t9 0x1e8:rt:t2:t0 0x1f0:rd:t3:t1 0x1f0:rt:t2:t0 0x1f4:rt:t3:t1 0x1fc:rt:t4:t2 0x204:rd:t5:t3 0x204:rt:t4:t2 0x208:rd:t6:t4 0x208:rt:t5:t3 0x20c:rt:t6:t4 0x234:rt:t7:t5 0x238:rs:t7:t5 0x238:rt:t7:t5 0x23c:rt:v0:v1 0x240:rd:t8:t6 0x240:rt:t7:t5 0x244:rs:v0:v1 0x244:rt:t8:t6 0x248:rt:t9:v0 0x25c:rs:t9:v0 0x25c:rt:t9:v0 0x260:rs:t9:v0 0x260:rt:t0:t7 0x264:rt:t2:t9 0x268:rd:t1:t8 0x268:rt:t0:t7 0x26c:rs:v0:v1 0x26c:rt:t1:t8 0x274:rs:t2:t9 0x274:rt:t2:t9 0x27c:rd:t3:t0 0x27c:rt:t2:t9 0x280:rd:t4:t1 0x280:rt:t3:t0 0x284:rt:t4:t1 0x2ac:rt:t5:t2 0x2b0:rs:t5:t2 0x2b0:rt:t5:t2 0x2b8:rd:t6:t3 0x2b8:rt:t5:t2 0x2bc:rt:t6:t3 0x2d8:rt:t7:t4 0x2e0:rd:t8:t5 0x2e0:rt:t7:t4 0x2e4:rt:t8:t5 0x2ec:rt:t9:t6 0x2f4:rd:t0:t7 0x2f4:rt:t9:t6 0x2f8:rd:t1:t8 0x2f8:rt:t0:t7 0x2fc:rt:t1:t8 0x334:rt:t2:t9 0x338:rs:t2:t9 0x340:rd:t2:t9 0x340:rt:t2:t9 0x348:rt:t2:t9 0x34c:rt:t2:t9 0x354:rs:t2:t9 */
/* Address: 0x80145F04 (window starts at the absorbed cross-prologue head */
/* Size: 988 bytes (247 insns) + 8-byte head = 996 */
#include "types.h"

extern s32 goto_sub_step();
extern s32 game_reset();

extern u8  screen_w[];
extern u8  girl0_basyo;
extern s32 girl1_flag;
extern s32 girl2_flag;
extern s32 girl3_flag;
extern s32 bunka_bukatu;
extern s32 bunka_girl;

void bunkaken_girl_route_step(void) {
    s32 sel;

    sel = bunka_girl;
    if (sel == 255) {
            switch ((u32)bunka_bukatu) {
                case 0: {
                    u8 *base;
                    base = (u8*)((u32)screen_w | 0);
                    if (0 == ((u32)girl0_basyo >> 4)) {
                        goto_sub_step(32);
                    } else if (0 == ((u32)base[514] >> 4) &&
                               1 == ((u32)(*(s32*)(base + 512) << 30) >> 31)) {
                        goto_sub_step(44);
                    } else {
                        goto_sub_step(5);
                    }
                    break;
                }

                case 1: {
                    u8 *base;
                    base = (u8*)((u32)screen_w | 0);
                    if (1 == ((u32)girl0_basyo >> 4)) {
                        goto_sub_step(32);
                    } else if (1 == ((u32)base[514] >> 4) &&
                               1 == ((u32)(girl1_flag << 30) >> 31)) {
                        goto_sub_step(44);
                    } else {
                        goto_sub_step(5);
                    }
                    break;
                }

                case 2: {
                    u8 *base;
                    base = (u8*)((u32)screen_w | 0);
                    if (2 == ((u32)girl0_basyo >> 4)) {
                        goto_sub_step(32);
                    } else if (2 == ((u32)base[570] >> 4) &&
                               1 == ((u32)(girl2_flag << 30) >> 31)) {
                        goto_sub_step(48);
                    } else {
                        goto_sub_step(9);
                    }
                    break;
                }

                case 3: {
                    u8 *base;
                    base = (u8*)((u32)screen_w | 0);
                    if (3 == ((u32)girl0_basyo >> 4)) {
                        goto_sub_step(32);
                    } else if (3 == ((u32)base[570] >> 4) &&
                               1 == ((u32)(*(s32*)(base + 568) << 30) >> 31)) {
                        goto_sub_step(48);
                    } else {
                        goto_sub_step(5);
                    }
                    break;
                }

                case 4: {
                    u8 *base;
                    base = (u8*)((u32)screen_w | 0);
                    if (4 == ((u32)girl0_basyo >> 4)) {
                        goto_sub_step(32);
                    } else if (4 == ((u32)base[626] >> 4) &&
                               1 == ((u32)(girl3_flag << 30) >> 31)) {
                        goto_sub_step(53);
                    } else {
                        goto_sub_step(5);
                    }
                    break;
                }

                case 5: {
                    u8 *base;
                    base = (u8*)((u32)screen_w | 0);
                    if (5 == ((u32)girl0_basyo >> 4)) {
                        goto_sub_step(32);
                    } else if (5 == ((u32)base[626] >> 4) &&
                               1 == ((u32)(*(s32*)(base + 624) << 30) >> 31)) {
                        goto_sub_step(53);
                    } else {
                        goto_sub_step(5);
                    }
                    break;
                }

                default:
                    game_reset();
                    break;
            }
    } else {
        switch ((u32)sel) {
            case 11:
                goto_sub_step(18);
                break;

            case 4:
                goto_sub_step(58);
                break;

            case 5:
                goto_sub_step(72);
                break;

            case 6:
                goto_sub_step(85);
                break;

            case 7:
                goto_sub_step(98);
                break;

            case 8:
                goto_sub_step(111);
                break;

            case 9:
                goto_sub_step(122);
                break;

            case 10:
                goto_sub_step(131);
                break;

            default:
                game_reset();
                break;
        }
    }
}
