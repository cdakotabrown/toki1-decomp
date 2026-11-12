/* @SymbolStart: 0x80142BE8 */
/* @SymbolSize: 1516 */
/* @PostProc: patch_stack_complete --old 304 --new 296 */
/* @PostProc: swap_instructions --offset1 0x11c --offset2 0x164 */
/* @PostProc: swap_instructions --offset1 0x130 --offset2 0x164 */
/* @PostProc: swap_instructions --offset1 0x134 --offset2 0x13c */
/* @PostProc: swap_instructions --offset1 0x138 --offset2 0x13c */
/* @PostProc: swap_instructions --offset1 0x13c --offset2 0x164 */
/* @PostProc: swap_instructions --offset1 0x140 --offset2 0x164 */
/* @PostProc: swap_instructions --offset1 0x144 --offset2 0x164 */
/* @PostProc: swap_instructions --offset1 0x148 --offset2 0x164 */
/* @PostProc: swap_instructions --offset1 0x14c --offset2 0x158 */
/* @PostProc: swap_instructions --offset1 0x150 --offset2 0x15c */
/* @PostProc: swap_instructions --offset1 0x154 --offset2 0x160 */
/* @PostProc: swap_instructions --offset1 0x168 --offset2 0x1a0 */
/* @PostProc: swap_instructions --offset1 0x16c --offset2 0x170 */
/* @PostProc: swap_instructions --offset1 0x170 --offset2 0x1a0 */
/* @PostProc: swap_instructions --offset1 0x174 --offset2 0x1a0 */
/* @PostProc: swap_instructions --offset1 0x178 --offset2 0x1a0 */
/* @PostProc: swap_instructions --offset1 0x180 --offset2 0x1a0 */
/* @PostProc: swap_instructions --offset1 0x184 --offset2 0x1a0 */
/* @PostProc: swap_instructions --offset1 0x188 --offset2 0x1a0 */
/* @PostProc: swap_instructions --offset1 0x18c --offset2 0x1a0 */
/* @PostProc: swap_instructions --offset1 0x190 --offset2 0x1a0 */
/* @PostProc: swap_instructions --offset1 0x194 --offset2 0x1a0 */
/* @PostProc: swap_instructions --offset1 0x198 --offset2 0x1a0 */
/* @PostProc: swap_instructions --offset1 0x19c --offset2 0x1a0 */
/* @PostProc: swap_instructions --offset1 0x23C --offset2 0x240 */
/* @PostProc: swap_instructions --offset1 0x348 --offset2 0x34C */
/* @PostProc: swap_instructions --offset1 0x388 --offset2 0x38C */
/* @PostProc: swap_instructions --offset1 0x494 --offset2 0x498 */
/* @PostProc: swap_instructions --offset1 0x4D4 --offset2 0x4D8 */
/* @PostProc: remap_stack_slots --map '292:284,284:276,210:202,204:196,130:122,124:116,50:42,44:36' */
/* @PostProc: patch_insn_reg_field 0x20:rt:t2:t0 0x24:rs:t2:t0 0x2c:rt:t3:t4 0x38:rs:t3:t4 0x38:rt:t3:t4 0x48:rs:t3:t4 0x48:rt:t8:t7 0x4c:rd:t7:t6 0x50:rs:t2:t0 0x54:rs:t3:t4 0x58:rs:t3:t4 0x58:rt:t3:t4 0x5c:rs:t7:t6 0x60:rs:t3:t4 0x64:rs:t7:t6 0x64:rt:t7:t6 0x68:rs:t7:t6 0x6c:rs:t3:t4 0x70:rs:t3:t4 0x70:rt:t8:t7 0x74:rs:t7:t6 0x78:rs:t3:t4 0x7c:rt:t6:t9 0x80:rs:t7:t6 0x84:rs:t3:t4 0x88:rs:t6:t9 0x88:rt:t6:t9 0x8c:rs:t6:t9 0x8c:rt:t4:t7 0x90:rd:t8:t4 0x94:rs:t7:t6 0x98:rs:t6:t9 0x9c:rs:t6:t9 0x9c:rt:t6:t9 0xa0:rs:t8:t4 0xa4:rs:t6:t9 0xa8:rs:t8:t4 0xa8:rt:t8:t4 0xac:rs:t8:t4 0xb0:rs:t6:t9 0xb4:rs:t6:t9 0xb4:rt:t4:t7 0xb8:rs:t8:t4 0xbc:rs:t6:t9 0xc0:rt:t3:t6 0xc4:rs:t8:t4 0xc8:rs:t6:t9 0xcc:rs:t3:t6 0xcc:rt:t3:t6 0xd0:rs:t3:t6 0xd0:rt:t9:t7 0xd4:rd:t4:t9 0xd8:rs:t8:t4 0xdc:rs:t3:t6 0xe0:rs:t3:t6 0xe0:rt:t3:t6 0xe4:rs:t4:t9 0xe8:rs:t3:t6 0xec:rs:t4:t9 0xec:rt:t4:t9 0xf0:rs:t4:t9 0xf4:rs:t3:t6 0xf8:rs:t3:t6 0xf8:rt:t9:t7 0xfc:rs:t4:t9 0x100:rs:t3:t6 0x108:rs:t4:t9 0x10c:rs:t3:t6 0x110:rt:t3:t2 0x114:rs:t4:t9 0x11c:rt:t2:t9 0x120:rd:t6:t4 0x124:rt:t6:t4 0x130:rs:t3:t9 0x130:rt:t3:t9 0x134:rs:t6:t4 0x138:rd:t9:t6 0x13c:rs:t3:t9 0x140:rs:t9:t6 0x144:rd:t6:t5 0x144:rs:t2:t0 0x148:rs:t6:t5 0x148:rt:t1:t7 0x158:rd:t8:t9 0x158:rt:t1:t7 0x15c:rd:t5:t6 0x15c:rt:t8:t9 0x160:rs:t5:t6 0x160:rt:t7:t4 0x174:rt:t8:t3 0x178:rd:t0:t8 0x184:rs:t7:t4 0x188:rd:t3:t5 0x18c:rs:t3:t5 0x18c:rt:t0:t8 0x190:rt:t9:t7 0x194:rs:t9:t7 0x19c:rs:t9:t7 0x1b0:rt:t4:t9 0x1b4:rs:t4:t9 0x1c0:rs:t4:t9 0x1c8:rs:t4:t9 0x1cc:rd:t7:t6 0x1e4:rt:t0:t1 0x1e8:rs:t8:t3 0x1e8:rt:t8:t3 0x1ec:rd:t7:t4 0x1ec:rt:t0:t1 0x1f0:rs:t7:t4 0x1f0:rt:t3:t5 0x1f4:rd:t5:t7 0x1f4:rs:t8:t3 0x1f8:rd:t9:t6 0x1f8:rs:t5:t7 0x1f8:rt:t3:t5 0x204:rt:t9:t6 0x214:rt:t7:t6 0x218:rd:t6:t5 0x21c:rt:t6:t5 0x220:rt:t0:t1 0x224:rd:t5:t4 0x224:rs:t7:t6 0x228:rt:t5:t4 0x22c:rt:t3:t5 0x230:rd:t9:t7 0x230:rt:t0:t1 0x234:rs:t9:t7 0x234:rt:t4:t9 0x238:rd:t6:t8 0x238:rs:t3:t5 0x238:rt:t4:t9 0x23c:rt:t8:t7 0x240:rt:t6:t8 0x244:rs:t8:t7 0x244:rt:t8:t7 0x248:rt:t9:t8 0x24c:rd:t7:t9 0x24c:rt:t8:t7 0x250:rd:t5:t6 0x250:rt:t7:t9 0x254:rs:t5:t6 0x254:rt:t0:t1 0x258:rd:t3:t7 0x258:rs:t9:t8 0x25c:rt:t3:t7 0x260:rt:t4:t9 0x264:rd:t6:t4 0x264:rt:t0:t1 0x268:rs:t6:t4 0x268:rt:t8:t5 0x26c:rd:t7:t6 0x26c:rs:t4:t9 0x26c:rt:t8:t5 0x270:rt:t7:t6 0x274:rt:t5:t4 0x278:rs:t5:t4 0x278:rt:t5:t4 0x280:rd:t9:t5 0x280:rt:t5:t4 0x284:rd:t3:t8 0x284:rt:t9:t5 0x288:rs:t3:t8 0x288:rt:t0:t1 0x294:rt:t8:t5 0x298:rt:t0:t1 0x29c:rt:t5:t9 0x2a0:rd:t9:t8 0x2a0:rs:t8:t5 0x2a0:rt:t5:t9 0x2a4:rt:t9:t8 0x2a8:rt:t3:t7 0x2ac:rs:t3:t7 0x2ac:rt:t3:t7 0x2b0:rt:t7:t8 0x2b4:rd:t6:t9 0x2b4:rt:t3:t7 0x2b8:rd:t4:t6 0x2b8:rt:t6:t9 0x2bc:rs:t4:t6 0x2bc:rt:t0:t1 0x2c0:rd:t8:t7 0x2c0:rs:t7:t8 0x2c4:rt:t8:t7 0x2c8:rt:t5:t9 0x2cc:rd:t9:t4 0x2cc:rt:t0:t1 0x2d0:rs:t9:t4 0x2d0:rt:t3:t5 0x2d8:rs:t5:t9 0x2d8:rt:t3:t5 0x304:rd:t7:t5 0x308:rd:t9:t7 0x30c:rd:t5:t9 0x30c:rs:t7:t5 0x30c:rt:t9:t7 0x310:rs:t5:t9 0x31c:rd:t3:t6 0x320:rs:t3:t6 0x320:rt:t0:t1 0x324:rt:t6:t3 0x328:rs:t6:t3 0x328:rt:t6:t3 0x32c:rd:t8:t4 0x32c:rt:t0:t1 0x330:rs:t8:t4 0x330:rt:t7:t8 0x334:rd:t4:t7 0x334:rs:t6:t3 0x338:rd:t9:t6 0x338:rs:t4:t7 0x338:rt:t7:t8 0x340:rt:at:t2 0x344:rt:t2:at 0x348:rt:t9:t6 0x358:rt:t5:t4 0x35c:rs:t5:t4 0x35c:rt:t5:t4 0x360:rt:t8:t6 0x364:rd:t3:t8 0x364:rt:t5:t4 0x368:rd:t6:t5 0x368:rt:t3:t8 0x36c:rs:t6:t5 0x36c:rt:t0:t1 0x370:rs:t8:t6 0x378:rt:t7:t8 0x37c:rd:t9:t7 0x37c:rt:t0:t1 0x380:rs:t9:t7 0x380:rt:t5:t9 0x384:rd:t3:t5 0x384:rs:t7:t8 0x384:rt:t5:t9 0x388:rt:t6:t7 0x38c:rt:t3:t5 0x390:rs:t6:t7 0x390:rt:t6:t7 0x394:rt:t9:t5 0x398:rd:t8:t9 0x398:rt:t6:t7 0x39c:rd:t4:t6 0x39c:rt:t8:t9 0x3a0:rs:t4:t6 0x3a0:rt:t0:t1 0x3a4:rs:t9:t5 0x3ac:rt:t5:t9 0x3b0:rd:t3:t4 0x3b0:rt:t0:t1 0x3b4:rs:t3:t4 0x3b4:rt:t6:t8 0x3b8:rd:t8:t6 0x3b8:rs:t5:t9 0x3b8:rt:t6:t8 0x3bc:rt:t8:t6 0x3c8:rt:t3:t6 0x3cc:rd:t9:t8 0x3d0:rd:t7:t5 0x3d0:rt:t9:t8 0x3d4:rs:t7:t5 0x3d4:rt:t0:t1 0x3d8:rd:t5:t4 0x3d8:rs:t3:t6 0x3dc:rt:t5:t4 0x3e0:rt:t6:t8 0x3e4:rd:t8:t7 0x3e4:rt:t0:t1 0x3e8:rs:t8:t7 0x3e8:rt:t4:t9 0x3ec:rd:t9:t5 0x3ec:rs:t6:t8 0x3ec:rt:t4:t9 0x3f0:rt:t9:t5 0x3fc:rt:t8:t5 0x400:rd:t3:t9 0x404:rd:t5:t6 0x404:rt:t3:t9 0x408:rs:t5:t6 0x408:rt:t0:t1 0x40c:rd:t6:t7 0x40c:rs:t8:t5 0x410:rt:t6:t7 0x414:rt:t4:t9 0x418:rd:t9:t4 0x418:rt:t0:t1 0x41c:rs:t9:t4 0x41c:rt:t7:t8 0x424:rd:t3:t6 0x424:rs:t4:t9 0x424:rt:t7:t8 0x428:rt:t3:t6 0x43c:rt:t6:t5 0x440:rd:t5:t4 0x444:rs:t6:t5 0x444:rt:t6:t5 0x448:rd:t5:t4 0x448:rs:t5:t4 0x44c:rd:t5:t4 0x44c:rt:t5:t4 0x450:rt:t5:t4 0x454:rd:t9:t7 0x454:rt:t6:t5 0x458:rd:t4:t9 0x458:rt:t9:t7 0x45c:rs:t4:t9 0x468:rd:t7:t6 0x46c:rs:t7:t6 0x46c:rt:t0:t1 0x478:rd:t6:t4 0x478:rt:t0:t1 0x47c:rs:t6:t4 0x47c:rt:t8:t5 0x480:rd:t5:t7 0x484:rd:t9:t6 0x484:rs:t5:t7 0x484:rt:t8:t5 0x48c:rt:at:t2 0x490:rt:t2:at 0x494:rt:t9:t6 0x4b0:rd:t7:t5 0x4b4:rd:t3:t8 0x4b4:rt:t7:t5 0x4b8:rs:t3:t8 0x4b8:rt:t0:t1 0x4bc:rd:t5:t4 0x4c0:rt:t5:t4 0x4c4:rt:t8:t5 0x4c8:rd:t9:t7 0x4c8:rt:t0:t1 0x4cc:rs:t9:t7 0x4cc:rt:t4:t9 0x4d0:rd:t7:t8 0x4d0:rs:t8:t5 0x4d0:rt:t4:t9 0x4d4:rt:t3:t7 0x4d8:rt:t7:t8 0x4dc:rs:t3:t7 0x4dc:rt:t3:t7 0x4e0:rt:t9:t8 0x4e4:rd:t6:t9 0x4e4:rt:t3:t7 0x4e8:rd:t5:t6 0x4e8:rt:t6:t9 0x4ec:rs:t5:t6 0x4ec:rt:t0:t1 0x4f0:rd:t8:t7 0x4f0:rs:t9:t8 0x4f4:rt:t8:t7 0x4f8:rt:t4:t9 0x4fc:rd:t7:t4 0x4fc:rt:t0:t1 0x500:rs:t7:t4 0x500:rt:t3:t5 0x504:rs:t4:t9 0x504:rt:t3:t5 0x50c:rt:t5:t4 0x510:rs:t5:t4 0x510:rt:t5:t4 0x514:rt:t7:t6 0x518:rd:t9:t5 0x518:rt:t5:t4 0x51c:rt:t9:t5 0x520:rt:t0:t1 0x524:rs:t7:t6 0x52c:rt:t3:t5 0x530:rd:t6:t7 0x530:rt:t0:t1 0x534:rs:t6:t7 0x534:rt:t5:t9 0x538:rd:t9:t8 0x538:rs:t3:t5 0x538:rt:t5:t9 0x53c:rt:t9:t8 0x540:rt:t8:t7 0x544:rs:t8:t7 0x544:rt:t8:t7 0x548:rt:t6:t8 0x54c:rd:t7:t9 0x54c:rt:t8:t7 0x550:rd:t4:t6 0x550:rt:t7:t9 0x554:rs:t4:t6 0x554:rt:t0:t1 0x558:rd:t3:t7 0x558:rs:t6:t8 0x55c:rt:t3:t7 0x560:rt:t5:t9 0x564:rd:t9:t4 0x564:rt:t0:t1 0x568:rs:t9:t4 0x568:rt:t8:t5 0x570:rd:t7:t6 0x570:rs:t5:t9 0x570:rt:t8:t5 0x574:rt:t7:t6 0x588:rt:t3:t8 0x590:rs:t3:t8 0x590:rt:t3:t8 0x59c:rd:t6:t5 0x5a0:rd:t9:t7 0x5a0:rt:t3:t8 0x5a4:rd:t5:t9 0x5a4:rs:t6:t5 0x5a4:rt:t9:t7 0x5a8:rs:t5:t9 0x5bc:rt:t8:t6 0x5c0:rs:t8:t6 0x5c8:rd:t7:t4 0x5cc:rt:t7:t4 0x5d0:rt:t4:t8 0x5d8:rs:t4:t8 0x5d8:rt:t3:t5 0x5dc:rt:t3:t5 0x5e0:rd:t6:t7 0x5e4:rt:t6:t7 0x5f0:rd:t5:t6 0x5f8:rt:t5:t6 */
/* Address: 0x80142BF4, size 1504 bytes (376 insns), plus the 12-byte */
#include "types.h"

typedef struct { u16 h[3]; } Rec6;
typedef struct { u16 h[39]; } Rec78;

extern u8 screen_w[];
extern volatile u8 now_year;
extern volatile u16 basyo_pack;
extern s16 girl0_tokimeki;
extern s32 bunka_girl;
extern Rec6 bunkasai_girl_para_add_tbl0;
extern Rec78 bunkasai_girl_para_add_tbl1;
extern Rec78 bunkasai_girl_para_add_tbl2;
extern Rec78 bunkasai_girl_para_add_tbl3;

#define ARM(BUF)                                                          \
    b = ((u8 *)&BUF)[idx];                                                \
    girl0_tokimeki = girl0_tokimeki + v0 + ((b >> 1) & 0xFF);                     \
    for (k = 1; k < 13; k++) {                                            \
        b = (((u8 *)&BUF) + k * 6)[(u32)basyo_pack >> 12];                \
        *(s16 *)(ap + 446 + k * 56) += v0;                                \
        *(s16 *)(ap + 446 + k * 56) += (b >> 1) & 0xFF;                   \
    }                                                                     \
    v1 = bunka_girl;                                                      \
    v0 = ((u8 *)&BUF)[v1 * 6 + ((u32)basyo_pack >> 12)];

void bunkasai_girl_para_add(void)
{
    Rec6 spare[2];
    Rec6 buf0;
    Rec78 buf1;
    Rec78 buf2;
    Rec78 buf3;
    u8 *ap;
    u32 idx;
    u32 c;
    u32 q;
    u32 v0;
    s32 v1;
    s32 k;
    u32 b;

    buf0 = bunkasai_girl_para_add_tbl0;
    buf1 = bunkasai_girl_para_add_tbl1;
    buf2 = bunkasai_girl_para_add_tbl2;
    buf3 = bunkasai_girl_para_add_tbl3;

    ap = screen_w;
    idx = (u32)basyo_pack >> 12;
    q = ((u32)*(u16 *)(screen_w + 1644 + idx * 4) >> 3) /
        (u32)(now_year - 94);
    c = ((u8 *)&buf0)[idx];
    v0 = (u32)(*(s16 *)(screen_w + 254 + c * 4) / 20) + q;
    v0 = v0 & 0xFF;
    if (v0 >= 11) {
        v0 = 10;
    }

    switch ((u8)(now_year - 95)) {
    case 0:
        ARM(buf1)
        break;
    case 1:
        ARM(buf2)
        break;
    case 2:
        ARM(buf3)
        break;
    default:
        v1 = bunka_girl;
        v0 = ((u8 *)spare)[0];
        break;
    }

    if (v0 != 0) {
        v0 = (v0 - 1) & 0xFF;
    }
    *(s16 *)(screen_w + 450 + (u32)v1 * 56U) += 2;
    *(s16 *)(screen_w + 446 + (u32)bunka_girl * 56U) += v0;
}
