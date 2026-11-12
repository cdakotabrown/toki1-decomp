/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all IDO52_UGEN_TEMP_REG=33:t1 */
/* @SymbolSize: 1740 */
/* Function: date_basyo_set */
/* Overlay: DATE */
/* Address: 0x801389A4 */
/* Size: 1740 bytes (435 instructions) */
/* @PostProc: swap_instructions --offset1 0xa0 --offset2 0xa8 */
/* @PostProc: swap_instructions --offset1 0xa4 --offset2 0xac */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xbc */
/* @PostProc: swap_instructions --offset1 0xb4 --offset2 0xbc */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xbc */
/* @PostProc: swap_instructions --offset1 0x1f0 --offset2 0x1f8 */
/* @PostProc: swap_instructions --offset1 0x1f4 --offset2 0x1fc */
/* @PostProc: swap_instructions --offset1 0x200 --offset2 0x20c */
/* @PostProc: swap_instructions --offset1 0x204 --offset2 0x20c */
/* @PostProc: swap_instructions --offset1 0x208 --offset2 0x20c */
/* @PostProc: swap_instructions --offset1 0x20c --offset2 0x210 */
/* @PostProc: swap_instructions --offset1 0x210 --offset2 0x214 */
/* @PostProc: swap_instructions --offset1 0x214 --offset2 0x218 */
/* @PostProc: swap_instructions --offset1 0x344 --offset2 0x34c */
/* @PostProc: swap_instructions --offset1 0x348 --offset2 0x350 */
/* @PostProc: swap_instructions --offset1 0x354 --offset2 0x360 */
/* @PostProc: swap_instructions --offset1 0x358 --offset2 0x360 */
/* @PostProc: swap_instructions --offset1 0x35c --offset2 0x360 */
/* @PostProc: swap_instructions --offset1 0x360 --offset2 0x364 */
/* @PostProc: swap_instructions --offset1 0x364 --offset2 0x368 */
/* @PostProc: swap_instructions --offset1 0x368 --offset2 0x36c */
/* @PostProc: swap_instructions --offset1 0x49c --offset2 0x4a4 */
/* @PostProc: swap_instructions --offset1 0x4a0 --offset2 0x4a8 */
/* @PostProc: swap_instructions --offset1 0x4ac --offset2 0x4b8 */
/* @PostProc: swap_instructions --offset1 0x4b0 --offset2 0x4b8 */
/* @PostProc: swap_instructions --offset1 0x4b4 --offset2 0x4b8 */
/* @PostProc: swap_instructions --offset1 0x4b8 --offset2 0x4bc */
/* @PostProc: swap_instructions --offset1 0x4bc --offset2 0x4c0 */
/* @PostProc: swap_instructions --offset1 0x4c0 --offset2 0x4c4 */
/* @PostProc: swap_instructions --offset1 0x5e4 --offset2 0x5ec */
/* @PostProc: swap_instructions --offset1 0x5e8 --offset2 0x5f0 */
/* @PostProc: swap_instructions --offset1 0x5f4 --offset2 0x600 */
/* @PostProc: swap_instructions --offset1 0x5f8 --offset2 0x600 */
/* @PostProc: swap_instructions --offset1 0x5fc --offset2 0x600 */
/* @PostProc: swap_instructions --offset1 0x600 --offset2 0x604 */
/* @PostProc: swap_instructions --offset1 0x604 --offset2 0x608 */
/* @PostProc: swap_instructions --offset1 0x608 --offset2 0x60c */
/* @PostProc: patch_insn_reg_field 0x2c:rt:v1:v0 0x30:rs:v1:v0 0x7c:rt:a1:t2 0x94:rs:a1:t2 0x94:rt:a1:t2 0x98:rt:a0:v1 0x9c:rs:a1:t2 0x9c:rt:a0:v1 0xa0:rt:t3:t4 0xa4:rs:t3:t4 0xa4:rt:t3:t4 0xc0:rd:t2:t3 0xc4:rd:t4:t5 0xc4:rs:t2:t3 0xc4:rt:t3:t4 0xc8:rd:t5:t6 0xc8:rs:t4:t5 0xcc:rs:a1:t2 0xcc:rt:a0:v1 0xd0:rs:t5:t6 0xd0:rt:t6:t7 0xd8:rt:t6:t7 0xe0:rd:t7:t8 0xe4:rd:t8:t9 0xe4:rs:t7:t8 0xe4:rt:t3:t4 0xe8:rd:t9:t0 0xe8:rs:t8:t9 0xec:rs:a1:t2 0xec:rt:a0:v1 0xf0:rs:t9:t0 0xf0:rt:t0:t1 0xf4:rt:t7:t9 0xf8:rs:t7:t9 0xf8:rt:t7:t9 0xfc:rt:t0:t1 0x104:rd:t1:t3 0x108:rd:t2:t5 0x108:rs:t1:t3 0x108:rt:t3:t4 0x10c:rd:t4:t6 0x10c:rs:t2:t5 0x110:rs:a1:t2 0x110:rt:a0:v1 0x114:rs:t4:t6 0x114:rt:t5:t7 0x11c:rt:t5:t7 0x124:rd:t6:t8 0x128:rd:t8:t0 0x128:rs:t6:t8 0x128:rt:t7:t9 0x12c:rd:t9:t1 0x12c:rs:t8:t0 0x130:rs:a1:t2 0x130:rt:a0:v1 0x134:rs:t9:t1 0x134:rt:t0:t3 0x13c:rt:t0:t3 0x144:rd:t1:t4 0x148:rd:t3:t5 0x148:rs:t1:t4 0x148:rt:t7:t9 0x14c:rd:t2:t6 0x14c:rs:t3:t5 0x150:rs:a1:t2 0x150:rt:a0:v1 0x154:rs:t2:t6 0x154:rt:t4:t7 0x158:rt:t1:t4 0x15c:rt:t4:t7 0x164:rd:t5:t8 0x168:rd:t6:t0 0x168:rs:t5:t8 0x168:rt:t7:t9 0x16c:rd:t8:t1 0x16c:rs:t6:t0 0x170:rs:t8:t1 0x170:rt:t9:t3 0x178:rt:t9:t3 0x184:rt:t1:t4 0x1a0:rt:t0:t5 0x1a4:rs:t0:t5 0x1a4:rt:t0:t5 0x1a8:rt:t2:t7 0x1ac:rd:t3:t6 0x1ac:rt:t0:t5 0x1b0:rd:t3:t6 0x1b0:rs:t3:t6 0x1b0:rt:t0:t5 0x1b4:rd:t3:t6 0x1b4:rt:t3:t6 0x1b8:rd:t2:t7 0x1b8:rs:t2:t7 0x1b8:rt:t3:t6 0x1bc:rs:t2:t7 0x1bc:rt:t2:t7 0x1c0:rt:t5:t8 0x1c4:rs:t2:t7 0x1c4:rt:t4:t2 0x1c8:rs:t4:t2 0x1cc:rt:a1:t1 0x1e0:rt:t5:t8 0x1e4:rs:a1:t1 0x1e4:rt:a1:t1 0x1e8:rt:a0:v1 0x1ec:rs:a1:t1 0x1ec:rt:a0:v1 0x1f0:rt:t9:t4 0x1f4:rs:t9:t4 0x1f4:rt:t9:t4 0x200:rt:t7:t9 0x204:rd:t6:t0 0x208:rd:t6:t0 0x208:rs:t6:t0 0x210:rt:t7:t9 0x218:rs:t6:t0 0x21c:rd:t8:t3 0x220:rd:t1:t5 0x220:rs:t8:t3 0x220:rt:t9:t4 0x224:rd:t0:t6 0x224:rs:t1:t5 0x224:rt:t6:t0 0x228:rs:a1:t1 0x228:rt:a0:v1 0x22c:rs:t0:t6 0x22c:rt:t3:t7 0x234:rt:t3:t7 0x240:rd:t4:t8 0x240:rt:t9:t4 0x244:rd:t5:t9 0x244:rs:t4:t8 0x244:rt:t6:t0 0x248:rs:a1:t1 0x248:rt:a0:v1 0x24c:rs:t5:t9 0x24c:rt:t7:t0 0x250:rt:t2:t8 0x254:rs:t2:t8 0x254:rt:t2:t8 0x258:rt:t7:t0 0x260:rd:t6:t3 0x264:rd:t8:t5 0x264:rs:t6:t3 0x264:rt:t9:t4 0x268:rd:t1:t6 0x268:rs:t8:t5 0x26c:rs:a1:t1 0x26c:rt:a0:v1 0x270:rs:t1:t6 0x270:rt:t0:t7 0x278:rt:t0:t7 0x280:rd:t3:t2 0x284:rd:t4:t9 0x284:rs:t3:t2 0x284:rt:t2:t8 0x288:rd:t5:t0 0x288:rs:t4:t9 0x28c:rs:a1:t1 0x28c:rt:a0:v1 0x290:rs:t5:t0 0x290:rt:t7:t3 0x298:rt:t7:t3 0x2a0:rd:t6:t4 0x2a4:rd:t9:t5 0x2a4:rs:t6:t4 0x2a4:rt:t2:t8 0x2a8:rd:t8:t6 0x2a8:rs:t9:t5 0x2ac:rs:a1:t1 0x2ac:rt:a0:v1 0x2b0:rs:t8:t6 0x2b0:rt:t1:t7 0x2b8:rt:t1:t7 0x2c0:rd:t0:t2 0x2c4:rd:t3:t9 0x2c4:rs:t0:t2 0x2c4:rt:t2:t8 0x2c8:rd:t4:t0 0x2c8:rs:t3:t9 0x2cc:rs:t4:t0 0x2cc:rt:t5:t3 0x2d4:rt:t5:t3 0x2f4:rt:t7:t4 0x2f8:rs:t7:t4 0x2f8:rt:t7:t4 0x2fc:rt:t9:t6 0x300:rd:t6:t5 0x300:rt:t7:t4 0x304:rd:t6:t5 0x304:rs:t6:t5 0x304:rt:t7:t4 0x308:rd:t6:t5 0x308:rt:t6:t5 0x30c:rd:t9:t6 0x30c:rs:t9:t6 0x30c:rt:t6:t5 0x310:rs:t9:t6 0x310:rt:t9:t6 0x318:rs:t9:t6 0x318:rt:t8:t7 0x31c:rs:t8:t7 0x320:rt:a1:t9 0x338:rs:a1:t9 0x338:rt:a1:t9 0x33c:rt:a0:v1 0x340:rs:a1:t9 0x340:rt:a0:v1 0x344:rt:t4:t3 0x348:rs:t4:t3 0x348:rt:t4:t3 0x354:rt:t0:t2 0x358:rd:t2:t8 0x35c:rd:t2:t8 0x35c:rs:t2:t8 0x364:rt:t0:t2 0x36c:rs:t2:t8 0x370:rd:t3:t0 0x374:rd:t5:t4 0x374:rs:t3:t0 0x374:rt:t4:t3 0x378:rd:t7:t5 0x378:rs:t5:t4 0x378:rt:t2:t8 0x37c:rs:a1:t9 0x37c:rt:a0:v1 0x380:rs:t7:t5 0x390:rd:t9:t7 0x394:rd:t8:t1 0x394:rs:t9:t7 0x394:rt:t4:t3 0x398:rd:t1:t2 0x398:rs:t8:t1 0x398:rt:t2:t8 0x39c:rs:a1:t9 0x39c:rt:a0:v1 0x3a0:rs:t1:t2 0x3a0:rt:t0:t8 0x3a4:rt:t9:t1 0x3a8:rs:t9:t1 0x3a8:rt:t9:t1 0x3ac:rt:t0:t8 0x3b4:rd:t2:t0 0x3b8:rd:t3:t4 0x3b8:rs:t2:t0 0x3b8:rt:t4:t3 0x3bc:rs:t3:t4 0x3c0:rs:a1:t9 0x3c0:rt:a0:v1 0x3c4:rt:t7:t6 0x3cc:rt:t7:t6 0x3d4:rd:t6:t7 0x3d8:rd:t8:t2 0x3d8:rs:t6:t7 0x3d8:rt:t9:t1 0x3dc:rd:t1:t8 0x3dc:rs:t8:t2 0x3e0:rs:a1:t9 0x3e0:rt:a0:v1 0x3e4:rs:t1:t8 0x3f4:rd:t2:t3 0x3f8:rs:t2:t3 0x3f8:rt:t9:t1 0x3fc:rd:t3:t5 0x400:rs:a1:t9 0x400:rt:a0:v1 0x404:rs:t3:t5 0x404:rt:t5:t6 0x40c:rt:t5:t6 0x418:rd:t6:t2 0x418:rt:t9:t1 0x41c:rs:t6:t2 0x420:rt:t1:t0 0x428:rt:t1:t0 0x44c:rt:t0:t3 0x450:rs:t0:t3 0x450:rt:t0:t3 0x454:rt:t4:t5 0x458:rd:t2:t4 0x458:rt:t0:t3 0x45c:rd:t2:t4 0x45c:rs:t2:t4 0x45c:rt:t0:t3 0x460:rd:t2:t4 0x460:rt:t2:t4 0x464:rd:t4:t5 0x464:rs:t4:t5 0x464:rt:t2:t4 0x468:rs:t4:t5 0x468:rt:t4:t5 0x46c:rt:t5:t9 0x470:rs:t4:t5 0x470:rt:t3:t6 0x474:rs:t3:t6 0x478:rt:a1:t2 0x48c:rt:t5:t9 0x490:rs:a1:t2 0x490:rt:a1:t2 0x494:rt:a0:v1 0x498:rs:a1:t2 0x498:rt:a0:v1 0x49c:rt:t8:t0 0x4a0:rs:t8:t0 0x4a0:rt:t8:t0 0x4b0:rd:t9:t1 0x4b4:rd:t9:t1 0x4b4:rs:t9:t1 0x4c4:rs:t9:t1 0x4c8:rd:t6:t8 0x4cc:rd:t1:t3 0x4cc:rs:t6:t8 0x4cc:rt:t8:t0 0x4d0:rd:t0:t4 0x4d0:rs:t1:t3 0x4d0:rt:t9:t1 0x4d4:rs:a1:t2 0x4d4:rt:a0:v1 0x4d8:rs:t0:t4 0x4d8:rt:t2:t5 0x4e0:rt:t2:t5 0x4e8:rd:t4:t6 0x4ec:rd:t3:t9 0x4ec:rs:t4:t6 0x4ec:rt:t8:t0 0x4f0:rd:t5:t7 0x4f0:rs:t3:t9 0x4f0:rt:t9:t1 0x4f4:rs:a1:t2 0x4f4:rt:a0:v1 0x4f8:rs:t5:t7 0x4f8:rt:t7:t1 0x4fc:rt:t4:t9 0x500:rs:t4:t9 0x500:rt:t4:t9 0x504:rt:t7:t1 0x50c:rd:t9:t8 0x510:rd:t6:t3 0x510:rs:t9:t8 0x510:rt:t8:t0 0x514:rd:t1:t4 0x514:rs:t6:t3 0x518:rs:a1:t2 0x518:rt:a0:v1 0x51c:rs:t1:t4 0x51c:rt:t0:t5 0x524:rt:t0:t5 0x52c:rd:t2:t6 0x530:rd:t3:t7 0x530:rs:t2:t6 0x530:rt:t4:t9 0x534:rd:t5:t1 0x534:rs:t3:t7 0x538:rs:a1:t2 0x538:rt:a0:v1 0x53c:rs:t5:t1 0x53c:rt:t7:t8 0x544:rt:t7:t8 0x54c:rd:t9:t0 0x550:rd:t8:t3 0x550:rs:t9:t0 0x550:rt:t4:t9 0x554:rd:t6:t4 0x554:rs:t8:t3 0x558:rs:a1:t2 0x558:rt:a0:v1 0x55c:rs:t6:t4 0x55c:rt:t1:t5 0x564:rt:t1:t5 0x56c:rd:t0:t6 0x570:rd:t2:t7 0x570:rs:t0:t6 0x570:rt:t4:t9 0x574:rd:t3:t1 0x574:rs:t2:t7 0x578:rs:t3:t1 0x578:rt:t5:t8 0x580:rt:t5:t8 0x594:rt:t7:t0 0x598:rs:t7:t0 0x598:rt:t7:t0 0x59c:rt:t8:t4 0x5a0:rd:t9:t3 0x5a0:rt:t7:t0 0x5a4:rd:t9:t3 0x5a4:rs:t9:t3 0x5a4:rt:t7:t0 0x5a8:rd:t9:t3 0x5a8:rt:t9:t3 0x5ac:rd:t8:t4 0x5ac:rs:t8:t4 0x5ac:rt:t9:t3 0x5b0:rs:t8:t4 0x5b0:rt:t8:t4 0x5b4:rt:t1:t2 0x5b8:rs:t8:t4 0x5b8:rt:t6:t5 0x5bc:rs:t6:t5 0x5c0:rt:a1:t7 0x5d4:rt:t1:t2 0x5d8:rs:a1:t7 0x5d8:rt:a1:t7 0x5dc:rt:a0:v1 0x5e0:rs:a1:t7 0x5e0:rt:a0:v1 0x5e4:rt:t3:t8 0x5e8:rs:t3:t8 0x5e8:rt:t3:t8 0x5f4:rt:t0:t6 0x5f8:rd:t4:t9 0x5fc:rd:t4:t9 0x5fc:rs:t4:t9 0x604:rt:t0:t6 0x60c:rs:t4:t9 0x610:rd:t2:t1 0x614:rd:t5:t0 0x614:rs:t2:t1 0x614:rt:t3:t8 0x618:rd:t7:t3 0x618:rs:t5:t0 0x618:rt:t4:t9 0x61c:rs:a1:t7 0x61c:rt:a0:v1 0x620:rs:t7:t3 0x620:rt:t9:t4 0x628:rt:t9:t4 0x630:rd:t8:t5 0x634:rd:t6:t2 0x634:rs:t8:t5 0x634:rt:t3:t8 0x638:rd:t1:t6 0x638:rs:t6:t2 0x638:rt:t4:t9 0x63c:rs:a1:t7 0x63c:rt:a0:v1 0x640:rs:t1:t6 0x640:rt:t0:t9 0x644:rt:t8:t2 0x648:rs:t8:t2 0x648:rt:t8:t2 0x64c:rt:t0:t9 0x654:rd:t4:t1 0x658:rd:t2:t0 0x658:rs:t4:t1 0x658:rt:t3:t8 0x65c:rd:t5:t3 0x65c:rs:t2:t0 0x660:rs:a1:t7 0x660:rt:a0:v1 0x664:rs:t5:t3 0x664:rt:t7:t4 0x66c:rt:t7:t4 0x674:rd:t9:t5 0x678:rs:t9:t5 0x678:rt:t8:t2 0x67c:rd:t1:t9 0x680:rs:a1:t7 0x680:rt:a0:v1 0x684:rs:t1:t9 0x684:rt:t0:t1 0x68c:rt:t0:t1 0x694:rd:t4:t8 0x698:rd:t3:t0 0x698:rs:t4:t8 0x698:rt:t8:t2 0x69c:rd:t2:t3 0x69c:rs:t3:t0 0x6a0:rs:a1:t7 0x6a0:rt:a0:v1 0x6a4:rs:t2:t3 0x6a4:rt:t5:t4 0x6ac:rt:t5:t4 0x6b4:rd:t7:t5 0x6b8:rd:t9:t6 0x6b8:rs:t7:t5 0x6b8:rt:t8:t2 0x6bc:rd:t6:t9 0x6bc:rs:t9:t6 0x6c0:rs:t6:t9 */
#include "types.h"

extern void strcpy(void *, void *);

extern s16 kao[];
extern s16 imp[];
extern u8  ba_string;
extern u8  ba_string2;
extern volatile u8 date_pattern;
extern u8  girl0_spot21_cnt[];
extern u8  girl0_spot22_cnt[];
extern u8  girl0_spot23_cnt[];
extern u8  girl0_spot24_cnt[];
extern u8  girl0_spot25_cnt[];
extern volatile u8 now_girl;
extern s32 now_date_spot;
extern u8  date_basyo_set_str0;
extern u8  date_basyo_set_str1;
extern u8  date_basyo_set_str2;
extern u8  date_basyo_set_str3;
extern u8  date_basyo_set_str4;
extern u8  date_basyo_set_str5;
extern u8 *date_impkao_tbl_18_imp;
extern u8 *date_impkao_tbl_18_kao;
extern u8  date_ride_kind;

void date_basyo_set(void)
{
    u8 sel;
    u8 gv;
    u8 *p1;
    u8 *p2;
    s32 c3;

    strcpy(&ba_string2, &date_basyo_set_str0);

    sel = date_ride_kind;
    if (sel == 0) {
        strcpy(&ba_string, &date_basyo_set_str1);

        if ((girl0_spot21_cnt[now_girl * 56] & 1) == 0) {
            *(u8 *)&date_pattern = 0;
        } else {
            *(u8 *)&date_pattern = 1;
        }

        gv = now_girl;
        p1 = date_impkao_tbl_18_imp;
        c3 = date_pattern * 3;
        imp[0] = (p1 + gv * 36U)[c3 + 0];
        imp[1] = (p1 + (u32)gv * 36U)[c3 + 1];
        p2 = date_impkao_tbl_18_kao;
        imp[2] = (p1 + (gv | 0U) * 36U)[c3 + 2];
        kao[0] = (p2 + (gv ^ 0U) * 36U)[c3 + 0];
        kao[1] = (p2 + (gv * 1U) * 36U)[c3 + 1];
        kao[2] = (p2 + (gv >> 0) * 36U)[c3 + 2];
        now_date_spot = 21;
    } else if (sel == 3) {
        strcpy(&ba_string, &date_basyo_set_str2);

        if ((girl0_spot24_cnt[now_girl * 56] & 1) == 0) {
            *(u8 *)&date_pattern = 0;
        } else {
            *(u8 *)&date_pattern = 1;
        }

        gv = now_girl;
        p1 = date_impkao_tbl_18_imp;
        now_date_spot = 24;
        c3 = date_pattern * 3;
        imp[0] = (p1 + gv * 36U)[c3 + 18];
        imp[1] = (p1 + (u32)gv * 36U)[c3 + 19];
        p2 = date_impkao_tbl_18_kao;
        imp[2] = (p1 + (gv | 0U) * 36U)[c3 + 20];
        kao[0] = (p2 + (gv ^ 0U) * 36U)[c3 + 18];
        kao[1] = (p2 + (gv * 1U) * 36U)[c3 + 19];
        kao[2] = (p2 + (gv >> 0) * 36U)[c3 + 20];
    } else if (sel == 1) {
        strcpy(&ba_string, &date_basyo_set_str3);

        if ((girl0_spot22_cnt[now_girl * 56] & 1) == 0) {
            *(u8 *)&date_pattern = 0;
        } else {
            *(u8 *)&date_pattern = 1;
        }

        gv = now_girl;
        p1 = date_impkao_tbl_18_imp;
        now_date_spot = 22;
        c3 = date_pattern * 3;
        imp[0] = (p1 + gv * 36U)[c3 + 6];
        imp[1] = (p1 + (u32)gv * 36U)[c3 + 7];
        p2 = date_impkao_tbl_18_kao;
        imp[2] = (p1 + (gv | 0U) * 36U)[c3 + 8];
        kao[0] = (p2 + (gv ^ 0U) * 36U)[c3 + 6];
        kao[1] = (p2 + (gv * 1U) * 36U)[c3 + 7];
        kao[2] = (p2 + (gv >> 0) * 36U)[c3 + 8];
    } else if (sel == 4) {
        strcpy(&ba_string, &date_basyo_set_str4);

        if ((girl0_spot25_cnt[now_girl * 56] & 1) == 0) {
            *(u8 *)&date_pattern = 0;
        } else {
            *(u8 *)&date_pattern = 1;
        }

        gv = now_girl;
        p1 = date_impkao_tbl_18_imp;
        now_date_spot = 25;
        c3 = date_pattern * 3;
        imp[0] = (p1 + gv * 36U)[c3 + 24];
        imp[1] = (p1 + (u32)gv * 36U)[c3 + 25];
        p2 = date_impkao_tbl_18_kao;
        imp[2] = (p1 + (gv | 0U) * 36U)[c3 + 26];
        kao[0] = (p2 + (gv ^ 0U) * 36U)[c3 + 24];
        kao[1] = (p2 + (gv * 1U) * 36U)[c3 + 25];
        kao[2] = (p2 + (gv >> 0) * 36U)[c3 + 26];
    } else {
        strcpy(&ba_string, &date_basyo_set_str5);

        if ((girl0_spot23_cnt[now_girl * 56] & 1) == 0) {
            *(u8 *)&date_pattern = 0;
        } else {
            *(u8 *)&date_pattern = 1;
        }

        gv = now_girl;
        p1 = date_impkao_tbl_18_imp;
        now_date_spot = 23;
        c3 = date_pattern * 3;
        imp[0] = (p1 + gv * 36U)[c3 + 12];
        imp[1] = (p1 + (u32)gv * 36U)[c3 + 13];
        p2 = date_impkao_tbl_18_kao;
        imp[2] = (p1 + (gv | 0U) * 36U)[c3 + 14];
        kao[0] = (p2 + (gv ^ 0U) * 36U)[c3 + 12];
        kao[1] = (p2 + (gv * 1U) * 36U)[c3 + 13];
        kao[2] = (p2 + (gv >> 0) * 36U)[c3 + 14];
    }
}
