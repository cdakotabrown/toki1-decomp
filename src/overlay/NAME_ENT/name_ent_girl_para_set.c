/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
/* @PostProc: patch_stack_complete --old 72 --new 56 */
/* @PostProc: remap_stack_slots --from 28 --to 44 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x80 */
/* @PostProc: patch_insn_reg_field 0x28:rt:v1:t0 0x2c:rs:v1:t0 0x2c:rt:v1:t0 0x34:rd:t6:a2 0x34:rt:v1:t0 0x38:rd:t7:t6 0x38:rt:t6:a2 0x3c:rd:t8:t7 0x3c:rt:t7:t6 0x40:rd:t8:t7 0x40:rs:t8:t7 0x40:rt:t7:t6 0x44:rs:v1:t0 0x44:rt:t9:v1 0x48:rd:t1:t8 0x48:rs:t8:t7 0x48:rt:t9:v1 0x4c:rs:t1:t8 0x50:rt:t4:a3 0x54:rs:t4:a3 0x54:rt:t4:a3 0x5c:rs:t4:a3 0x5c:rt:t5:t4 0x68:rd:t2:t1 0x6c:rt:t2:t1 0x70:rd:v0:a3 0x70:rs:t5:t3 0x70:rt:t3:t4 0x74:rs:v0:a3 0x74:rt:t6:t5 0x78:rt:t6:t5 0x80:rt:t6:t5 0x84:rt:v0:t6 0x88:rs:v0:t6 0x88:rt:v0:t6 0x8c:rt:v1:t1 0x90:rs:v1:t1 0x90:rt:v1:t1 0x94:rt:v0:t6 0x9c:rd:t1:t2 0x9c:rt:v1:t1 0xa0:rd:t2:t3 0xa0:rt:t1:t2 0xa8:rt:t2:t3 0xac:rt:a3:a1 0xb0:rd:a3:a1 0xb0:rs:a3:a1 0xb4:rs:a3:a1 0xb4:rt:a3:a1 0xbc:rs:a3:a1 0xd0:rt:t3:t6 0xd4:rt:a3:a1 0xd8:rs:t3:t6 0xd8:rt:t3:t6 0xdc:rt:a3:a1 0xe0:rt:t3:t6 0xe4:rt:a0:t7 0xe8:rt:v0:a3 0xec:rs:v0:a3 0xec:rt:v0:a3 0xf4:rt:a0:t7 0xf8:rt:a1:t8 0x100:rt:a1:t8 0x104:rt:a2:t1 0x10c:rd:t0:a0 0x10c:rt:v0:a3 0x110:rs:t0:a0 0x114:rt:a2:t1 0x118:rt:v0:t2 0x11c:rs:v0:t2 0x11c:rt:v0:t2 0x124:rs:v0:t2 0x124:rt:t6:t9 0x128:rs:t6:t9 0x128:rt:t7:t3 0x130:rt:t7:t3 0x138:rs:t0:a0 0x13c:rt:v0:t4 0x140:rs:v0:t4 0x140:rt:v0:t4 0x148:rs:v0:t4 0x148:rt:t8:t5 0x150:rt:t8:t5 0x158:rt:t0:a0 0x15c:rt:t0:a0 0x160:rt:t1:t6 0x164:rs:t1:t6 0x168:rt:v0:t2 0x16c:rt:v0:t7 0x170:rs:v0:t7 0x170:rt:v0:t7 0x178:rs:v0:t7 0x178:rt:t9:t8 0x17c:rs:t9:t8 0x17c:rt:t2:t1 0x184:rt:t2:t1 0x188:rs:v0:t2 0x188:rt:v0:t2 0x190:rs:v0:t2 0x190:rt:t4:t9 0x194:rt:t4:t9 0x198:rt:v0:t3 0x19c:rs:v0:t3 0x19c:rt:v0:t3 0x1a4:rs:v0:t3 0x1a4:rt:t5:t4 0x1a8:rs:t5:t4 0x1a8:rt:t3:t5 0x1ac:rt:t3:t5 0x1b4:rs:t0:a0 0x1b8:rt:v0:t6 0x1bc:rs:v0:t6 0x1bc:rt:v0:t6 0x1c4:rs:v0:t6 0x1c4:rt:t6:t7 0x1c8:rs:t6:t7 0x1c8:rt:t7:t8 0x1d0:rt:t7:t8 0x1d8:rs:t0:a0 0x1dc:rt:v0:t1 0x1e0:rs:v0:t1 0x1e0:rt:v0:t1 0x1e8:rs:v0:t1 0x1e8:rt:t8:t2 0x1ec:rs:t8:t2 0x1ec:rt:t1:t9 0x1f4:rt:t1:t9 0x1fc:rt:t0:a0 0x200:rt:t0:a0 0x204:rt:t9:t3 0x208:rs:t9:t3 0x20c:rt:v0:t7 0x210:rt:v0:t4 0x214:rs:v0:t4 0x214:rt:v0:t4 0x21c:rs:v0:t4 0x21c:rt:t2:t5 0x220:rs:t2:t5 0x220:rt:t4:t6 0x228:rt:t4:t6 0x22c:rs:v0:t7 0x22c:rt:v0:t7 0x234:rs:v0:t7 0x234:rt:t5:t8 0x238:rs:t5:t8 0x238:rt:t3:t1 0x23c:rt:t3:t1 0x240:rt:v0:t2 0x244:rs:v0:t2 0x244:rt:v0:t2 0x24c:rs:v0:t2 0x24c:rt:t6:t9 0x250:rs:t6:t9 0x250:rt:t7:t3 0x254:rt:t7:t3 0x25c:rs:t0:a0 0x260:rt:v0:t4 0x264:rs:v0:t4 0x264:rt:v0:t4 0x26c:rs:v0:t4 0x26c:rt:t8:t5 0x270:rs:t8:t5 0x270:rt:t1:t6 0x278:rt:t1:t6 0x280:rs:t0:a0 0x284:rt:v0:t7 0x288:rs:v0:t7 0x288:rt:v0:t7 0x290:rs:v0:t7 0x290:rt:t9:t8 0x294:rs:t9:t8 0x294:rt:t2:t1 0x29c:rt:t2:t1 0x2a4:rt:t0:a0 0x2a8:rt:t0:a0 0x2ac:rt:t4:t2 0x2b0:rs:t4:t2 0x2b4:rt:v0:t5 0x2b8:rt:v0:t9 0x2bc:rs:v0:t9 0x2bc:rt:v0:t9 0x2c4:rs:v0:t9 0x2c4:rt:t5:t3 0x2c8:rs:t5:t3 0x2c8:rt:t3:t4 0x2d0:rt:t3:t4 0x2d4:rs:v0:t5 0x2d4:rt:v0:t5 0x2dc:rs:v0:t5 0x2e8:rt:v0:t8 0x2ec:rs:v0:t8 0x2ec:rt:v0:t8 0x2f4:rs:v0:t8 0x2f4:rt:t8:t1 0x2f8:rs:t8:t1 0x2f8:rt:t1:t2 0x2fc:rt:t1:t2 0x304:rs:t0:a0 0x308:rt:v0:t9 0x30c:rs:v0:t9 0x30c:rt:v0:t9 0x314:rs:v0:t9 0x314:rt:t9:t3 0x318:rs:t9:t3 0x318:rt:t2:t4 0x320:rt:t2:t4 0x328:rs:t0:a0 0x32c:rt:v0:t5 0x330:rs:v0:t5 0x330:rt:v0:t5 0x338:rs:v0:t5 0x338:rt:t4:t6 0x33c:rs:t4:t6 0x33c:rt:t5:t7 0x344:rt:t5:t7 0x350:rt:t3:t8 0x354:rs:t3:t8 0x358:rt:v0:t3 0x35c:rt:v0:t1 0x360:rs:v0:t1 0x360:rt:v0:t1 0x368:rs:v0:t1 0x368:rt:t6:t2 0x36c:rs:t6:t2 0x36c:rt:t7:t9 0x374:rt:t7:t9 0x378:rs:v0:t3 0x378:rt:v0:t3 0x380:rs:v0:t3 0x380:rt:t8:t4 0x384:rs:t8:t4 0x384:rt:t1:t5 0x388:rt:t1:t5 0x38c:rt:v0:t6 0x390:rs:v0:t6 0x390:rt:v0:t6 0x398:rs:v0:t6 0x398:rt:t9:t7 0x39c:rs:t9:t7 0x39c:rt:t2:t8 0x3a0:rt:t2:t8 0x3a4:rt:v0:t1 0x3a8:rs:v0:t1 0x3a8:rt:v0:t1 0x3b0:rs:v0:t1 0x3b0:rt:t4:t2 0x3b4:rs:t4:t2 0x3b4:rt:t5:t9 0x3b8:rt:t5:t9 0x3bc:rt:v0:t3 0x3c0:rs:v0:t3 0x3c0:rt:v0:t3 0x3c8:rs:v0:t3 0x3c8:rt:t3:t4 0x3cc:rs:t3:t4 0x3cc:rt:t6:t5 0x3d0:rt:t6:t5 0x3d4:rt:v0:t6 0x3d8:rs:v0:t6 0x3d8:rt:v0:t6 0x3e0:rs:v0:t6 0x3ec:rt:v0:t1 0x3f0:rs:v0:t1 0x3f0:rt:v0:t1 0x3f8:rs:v0:t1 0x3f8:rt:t1:t2 0x3fc:rs:t1:t2 0x404:rt:v0:t3 0x408:rs:v0:t3 0x408:rt:v0:t3 0x410:rs:v0:t3 0x410:rt:t2:t4 0x414:rs:t2:t4 0x414:rt:t4:t5 0x418:rt:t4:t5 0x41c:rt:v0:t6 0x420:rs:v0:t6 0x420:rt:v0:t6 0x428:rs:v0:t6 0x428:rt:t5:t7 0x42c:rt:t5:t7 0x430:rt:v0:t8 0x434:rs:v0:t8 0x434:rt:v0:t8 0x43c:rs:v0:t8 0x43c:rt:t3:t1 0x440:rs:t3:t1 0x440:rt:t6:t2 0x444:rt:t6:t2 0x448:rt:v0:t9 0x44c:rs:v0:t9 0x44c:rt:v0:t9 0x454:rs:v0:t9 0x454:rt:t7:t3 0x458:rs:t7:t3 0x458:rt:t8:t4 0x45c:rt:t8:t4 0xcc:rt:a3:a1 0x460:rt:v0:t5 0x464:rs:v0:t5 0x464:rt:v0:t5 0x46c:rs:v0:t5 0x46c:rt:t1:t6 0x470:rs:t1:t6 0x470:rt:t9:t7 0x474:rt:t9:t7 0x478:rt:v0:t8 0x47c:rs:v0:t8 0x47c:rt:v0:t8 0x484:rs:v0:t8 0x484:rt:t2:t1 0x488:rs:t2:t1 0x488:rt:t4:t2 0x48c:rt:t4:t2 0x490:rt:v0:t9 0x494:rs:v0:t9 0x494:rt:v0:t9 0x49c:rs:v0:t9 0x49c:rt:t5:t3 0x4a0:rs:t5:t3 0x4a0:rt:t3:t4 0x4a4:rt:t3:t4 0x4a8:rt:v0:t5 0x4ac:rs:v0:t5 0x4ac:rt:v0:t5 0x4b4:rs:v0:t5 0x4bc:rt:v0:t7 0x4c0:rs:v0:t7 0x4c0:rt:v0:t7 0x4c8:rs:v0:t7 0x4c8:rt:t7:t8 0x4cc:rs:t7:t8 0x4cc:rt:t8:t1 0x4d0:rt:t8:t1 0x4d4:rt:v0:t2 0x4d8:rs:v0:t2 0x4d8:rt:v0:t2 0x4e0:rs:v0:t2 0x4e0:rt:t1:t9 0x4e4:rs:t1:t9 0x4e4:rt:t9:t3 0x4e8:rt:t9:t3 0x4ec:rt:v0:t4 0x4f0:rs:v0:t4 0x4f0:rt:v0:t5 0x4f8:rs:v0:t5 0x4f8:rt:t2:t6 0x500:rt:t2:t6 */
/* name_ent_girl_para_set - NAME_ENT overlay, Address: 0x8013909C, Size: 1284 bytes (321 insns) */
#include "types.h"

extern volatile u32 girl0_birth;
extern volatile u8 girl0_basyo;
extern volatile u32 p_birthday;

extern volatile u8 name_ent_blood_aisyou_tbl[16];
extern volatile u8 name_ent_girl0_para_tbl[];

extern u8 name_ent_fujisaki_club_fmt[];
extern u8 name_ent_aisyo_fmt[];

extern volatile s16 girl0_yuukou;
extern volatile s16 girl0_tokimeki;
extern volatile s16 girl0_syoushin;

extern volatile u8 girl1_basyo;
extern volatile u8 girl1_birth_hi;
extern volatile u8 girl2_basyo;
extern volatile u8 girl2_birth_hi;
extern volatile u8 girl3_basyo;
extern volatile u8 girl3_birth_hi;
extern volatile u8 girl4_basyo;
extern volatile u8 girl4_birth_hi;
extern volatile u8 girl5_basyo;
extern volatile u8 girl5_birth_hi;
extern volatile u8 girl6_basyo;
extern volatile u8 girl6_birth_hi;
extern volatile u8 girl7_basyo;
extern volatile u8 girl7_birth_hi;
extern volatile u8 girl8_basyo;
extern volatile u8 girl8_birth_hi;
extern volatile u8 girl9_basyo;
extern volatile u8 girl9_birth_hi;
extern volatile u8 girl10_basyo;
extern volatile u8 girl10_birth_hi;
extern volatile u8 girl11_basyo;
extern volatile u8 girl11_birth_hi;
extern volatile u8 girl12_birth_hi;

extern void _game_work_init_set();
extern void name_ent_record_set();
extern void printf();
extern s32 rand();
extern void name_ent_mode_set();

void name_ent_girl_para_set(void)
{
    s32 randBit;
    u32 val1;
    u32 val2;
    u32 idx1;
    u32 idx2;
    u32 saveVal;
    u32 modeHigh;
    s32 slotSel;
    u32 tmpByte32;

    u32 tmpByte;
    s32 lookupIdx;
    u8 *datePtr;
    u32 d0;
    u32 d1;
    u32 d2;

    _game_work_init_set();
    name_ent_record_set();

    saveVal = girl0_birth;
    modeHigh = (saveVal << 23) >> 27;
    lookupIdx = (s32)((modeHigh * 3) + (saveVal & 0x0F)) % 11;

    tmpByte32 = (girl0_basyo & 0xFF0F) | (lookupIdx << 4);
    tmpByte32 = tmpByte32 & 0xFF;
    saveVal = tmpByte32 >> 4;
    girl0_basyo = tmpByte32;
    printf(name_ent_fujisaki_club_fmt, saveVal);

    val1 = p_birthday;
    val2 = girl0_birth;
    idx1 = (val1 << 21) >> 30;
    idx2 = (val2 << 21) >> 30;
    lookupIdx = name_ent_blood_aisyou_tbl[idx1 * 4 + idx2] - 1;

    printf(name_ent_aisyo_fmt, lookupIdx);

    datePtr = &name_ent_girl0_para_tbl[lookupIdx * 3];
    d0 = datePtr[0];
    tmpByte = girl0_basyo;
    girl0_yuukou = d0;
    d1 = datePtr[1];
    girl0_tokimeki = d1;
    d2 = datePtr[2];
    slotSel = (tmpByte >> 4) | 0;
    tmpByte = 0;
    girl0_syoushin = d2;

    if (slotSel == 0) {
        tmpByte = girl1_basyo;
        girl1_basyo = (tmpByte & 0xFF0F) | 0x10;
    } else if (slotSel == 1) {
        tmpByte = girl1_basyo;
        girl1_basyo = tmpByte & 0xFF0F;
    } else {
        randBit = rand() & 1;
        if (randBit) {
            tmpByte = girl1_basyo;
            girl1_basyo = (tmpByte & 0xFF0F) | 0x10;
        } else {
            tmpByte = girl1_basyo;
            girl1_basyo = tmpByte & 0xFF0F;
        }
    }

    tmpByte = girl1_birth_hi;
    girl1_birth_hi = (tmpByte & 0xFFF9) | 0x02;

    if (slotSel == 2) {
        tmpByte = girl2_basyo;
        girl2_basyo = (tmpByte & 0xFF0F) | 0x30;
    } else if (slotSel == 3) {
        tmpByte = girl2_basyo;
        girl2_basyo = (tmpByte & 0xFF0F) | 0x20;
    } else {
        randBit = rand() & 1;
        if (randBit) {
            tmpByte = girl2_basyo;
            girl2_basyo = (tmpByte & 0xFF0F) | 0x30;
        } else {
            tmpByte = girl2_basyo;
            girl2_basyo = (tmpByte & 0xFF0F) | 0x20;
        }
    }

    tmpByte = girl2_birth_hi;
    girl2_birth_hi = (tmpByte & 0xFFF9) | 0x02;

    if (slotSel == 4) {
        tmpByte = girl3_basyo;
        girl3_basyo = (tmpByte & 0xFF0F) | 0x50;
    } else if (slotSel == 5) {
        tmpByte = girl3_basyo;
        girl3_basyo = (tmpByte & 0xFF0F) | 0x40;
    } else {
        randBit = rand() & 1;
        if (randBit) {
            tmpByte = girl3_basyo;
            girl3_basyo = (tmpByte & 0xFF0F) | 0x50;
        } else {
            tmpByte = girl3_basyo;
            girl3_basyo = (tmpByte & 0xFF0F) | 0x40;
        }
    }

    tmpByte = girl3_birth_hi;
    girl3_birth_hi = (tmpByte & 0xFFF9) | 0x04;

    if (slotSel == 6) {
        tmpByte = girl4_basyo;
        girl4_basyo = (tmpByte & 0xFF0F) | 0x70;
    } else if (slotSel == 7) {
        tmpByte = girl4_basyo;
        girl4_basyo = (tmpByte & 0xFF0F) | 0x60;
    } else {
        randBit = rand() & 1;
        if (randBit) {
            tmpByte = girl4_basyo;
            girl4_basyo = (tmpByte & 0xFF0F) | 0x70;
        } else {
            tmpByte = girl4_basyo;
            girl4_basyo = (tmpByte & 0xFF0F) | 0x60;
        }
    }

    tmpByte = girl4_birth_hi;
    girl4_birth_hi = (tmpByte & 0xFFF9) | 0x02;

    tmpByte = girl5_basyo;
    girl5_basyo = (tmpByte & 0xFF0F) | 0x80;
    tmpByte = girl5_birth_hi;
    girl5_birth_hi = (tmpByte & 0xFFF9) | 0x04;

    tmpByte = girl6_basyo;
    girl6_basyo = (tmpByte & 0xFF0F) | 0x90;
    tmpByte = girl6_birth_hi;
    girl6_birth_hi = (tmpByte & 0xFFF9) | 0x02;

    tmpByte = girl7_basyo;
    girl7_basyo = (tmpByte & 0xFF0F) | 0xB0;
    tmpByte = girl7_birth_hi;
    girl7_birth_hi = tmpByte & 0xFFF9;

    tmpByte = girl8_basyo;
    girl8_basyo = (tmpByte & 0xFF0F) | 0xB0;
    tmpByte = girl8_birth_hi;
    girl8_birth_hi = (tmpByte & 0xFFF9) | 0x04;

    tmpByte = girl9_basyo;
    girl9_basyo = (tmpByte & 0xFF0F) | 0xB0;
    tmpByte = girl9_birth_hi;
    girl9_birth_hi = (tmpByte & 0xFFF9) | 0x02;

    tmpByte = girl10_basyo;
    girl10_basyo = (tmpByte & 0xFF0F) | 0xA0;
    tmpByte = girl10_birth_hi;
    girl10_birth_hi = tmpByte & 0xFFF9;

    tmpByte = girl11_basyo;
    girl11_basyo = (tmpByte & 0xFF0F) | 0xB0;
    tmpByte = girl11_birth_hi;
    girl11_birth_hi = (tmpByte & 0xFFF9) | 0x02;

    tmpByte = girl12_birth_hi;
    girl12_birth_hi = tmpByte | 0x06;

    name_ent_mode_set();
}
