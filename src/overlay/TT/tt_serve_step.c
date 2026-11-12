/* @RodataBase: 0x8014D6E0 */
/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: remap_stack_slots --map '64:92,76:104,80:108' */
/* @PostProc: swap_instructions --offset1 0xc0 --offset2 0xcc */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xd0 */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xcc */
/* @PostProc: swap_instructions --offset1 0xcc --offset2 0xd0 */
/* @PostProc: swap_instructions --offset1 0xd4 --offset2 0xdc */
/* @PostProc: swap_instructions --offset1 0xd8 --offset2 0xe0 */
/* @PostProc: swap_instructions --offset1 0xdc --offset2 0xe4 */
/* @PostProc: swap_instructions --offset1 0xe0 --offset2 0xe8 */
/* @PostProc: swap_instructions --offset1 0xe4 --offset2 0xec */
/* @PostProc: swap_instructions --offset1 0xe8 --offset2 0xf0 */
/* @PostProc: swap_instructions --offset1 0xec --offset2 0xf0 */
/* @PostProc: swap_instructions --offset1 0x4b4 --offset2 0x4b8 */
/* @PostProc: patch_insn_reg_field 0x5c:rt:t9:t7 0x78:rt:t9:t7 0xb8:rt:a1:t0 0xc0:rs:a1:t0 0xc0:rt:a1:t0 0xc4:rt:s0:a2 0xc8:rt:v0:v1 0xcc:rt:a0:a1 0xd0:rt:v1:a0 0xd4:rt:t3:ra 0xd8:rt:t2:t5 0xdc:rt:t1:t4 0xe0:rt:t0:t3 0xe4:rt:a3:t2 0xe8:rt:a2:t1 0xec:rt:t4:a3 0xf0:rt:t5:v0 0xf4:rs:v0:v1 0xf4:rt:a1:t0 0xf8:rs:v0:v1 0xfc:rs:v0:v1 0xfc:rt:a2:t1 0x100:rs:v0:v1 0x100:rt:a3:t2 0x104:rs:v0:v1 0x104:rt:t0:t3 0x108:rs:v0:v1 0x108:rt:t1:t4 0x10c:rs:v0:v1 0x110:rs:a0:a1 0x110:rt:t4:a3 0x114:rs:s0:a2 0x114:rt:s0:a2 0x118:rs:v0:v1 0x118:rt:v0:v1 0x11c:rs:a0:a1 0x11c:rt:a0:a1 0x120:rs:v1:a0 0x120:rt:v1:a0 0x124:rs:v1:a0 0x124:rt:t5:v0 0x128:rs:v1:a0 0x12c:rs:s0:a2 0x12c:rt:t3:ra 0x130:rs:v1:a0 0x130:rt:t2:t5 0x198:rt:v1:a0 0x1c4:rt:v0:t6 0x1c8:rt:a0:v0 0x1cc:rt:t7:t9 0x1d0:rs:v0:t6 0x1d0:rt:v0:v1 0x1d4:rd:t6:t8 0x1d4:rs:a0:a1 0x1d4:rt:a1:v0 0x1d8:rt:t6:t8 0x1dc:rd:t9:t7 0x1dc:rt:v0:v1 0x1e0:rd:t8:t6 0x1e0:rs:t7:v0 0x1e0:rt:a0:t9 0x1e4:rt:v0:v1 0x1e8:rt:t8:t6 0x1ec:rs:t9:t7 0x1f0:rd:t6:t8 0x1f0:rt:t9:t7 0x1f8:rt:t9:t7 0x1fc:rd:t6:t8 0x200:rt:t6:t8 0x204:rs:v0:v1 0x208:rd:t7:t9 0x208:rt:v0:v1 0x210:rt:v0:v1 0x214:rd:t7:t9 0x218:rt:t7:t9 0x21c:rt:v0:t6 0x224:rs:v0:t6 0x228:rd:t8:t7 0x228:rt:v0:t6 0x230:rt:v0:t6 0x234:rd:t8:t7 0x238:rs:v1:a0 0x238:rt:v1:a0 0x240:rs:v1:a0 0x240:rt:t8:t7 0x248:rt:t9:t7 0x24c:rt:t9:t8 0x258:rt:t9:t8 0x25c:rt:t6:t9 0x260:rs:t6:t9 0x260:rt:t6:t9 0x264:rt:t8:t7 0x268:rs:t6:t9 0x268:rt:t7:t6 0x270:rs:t7:t6 0x284:rt:t8:t7 0x288:rt:t4:a3 0x28c:rt:t9:t8 0x290:rt:t6:t9 0x29c:rt:t4:a3 0x2a0:rt:t9:t8 0x2a8:rt:t6:t9 0x2ac:rt:t7:t6 0x2b0:rs:t7:t6 0x2b0:rt:t7:t6 0x2b8:rs:t7:t6 0x2b8:rt:t8:t7 0x2c0:rt:t8:t7 0x2c8:rs:t7:t6 0x2c8:rt:t9:t8 0x2d0:rs:t9:t8 0x2d0:rt:t6:t9 0x2d4:rs:t6:t9 0x2e4:rt:t8:t7 0x2ec:rt:t8:t7 0x2f4:rt:t7:t6 0x2f8:rt:t5:v0 0x2fc:rs:t7:t6 0x2fc:rt:t9:t8 0x300:rs:t5:v0 0x300:rt:t9:t8 0x308:rt:t6:t9 0x310:rd:t8:t7 0x310:rt:t6:t9 0x314:rd:t7:t6 0x314:rs:t8:s2 0x314:rt:s2:t7 0x318:rs:t7:t6 0x318:rt:t9:t8 0x31c:rt:t6:t9 0x324:rd:t8:t7 0x324:rs:t9:t8 0x324:rt:t6:t9 0x328:rd:t7:t6 0x328:rt:t8:t7 0x32c:rt:t7:t6 0x330:rt:t9:t8 0x338:rd:t6:t9 0x338:rt:t9:t8 0x33c:rd:t8:t7 0x33c:rs:t6:s2 0x33c:rt:s2:t9 0x340:rs:t8:t7 0x340:rt:t7:t6 0x344:rt:t9:t8 0x34c:rd:t6:t9 0x34c:rs:t7:t6 0x34c:rt:t9:t8 0x350:rd:t8:t7 0x350:rt:t6:t9 0x358:rt:t8:t7 0x364:rt:t7:t6 0x368:rt:t8:t7 0x36c:rd:t9:t8 0x36c:rt:t7:t6 0x370:rs:t9:s2 0x370:rt:s2:t8 0x374:rt:t6:t9 0x378:rt:t7:t6 0x37c:rt:t9:t8 0x380:rs:t6:t9 0x380:rt:t8:t7 0x384:rt:t6:t9 0x388:rt:t6:t9 0x398:rs:t7:t6 0x398:rt:t9:t8 0x39c:rt:t8:t7 0x3a0:rt:t6:t9 0x3a4:rd:t7:t6 0x3a4:rt:t8:t7 0x3a8:rd:v0:v1 0x3a8:rs:t7:s2 0x3a8:rt:s2:t6 0x3ac:rs:v0:v1 0x3ac:rt:t9:t8 0x3b0:rs:v0:v1 0x3b0:rt:t8:t7 0x3b4:rt:t7:t6 0x3b8:rs:t9:t8 0x3b8:rt:t6:t9 0x3bc:rt:t9:t8 0x3c0:rt:t9:t8 0x3d0:rs:t8:t7 0x3d0:rt:t7:t6 0x3d4:rt:t6:t9 0x3d8:rt:t9:t8 0x3dc:rd:t8:t7 0x3dc:rt:t6:t9 0x3e0:rd:v0:v1 0x3e0:rs:t8:s2 0x3e0:rt:s2:t7 0x3e4:rs:v0:v1 0x3e4:rt:t7:t6 0x3e8:rs:v0:v1 0x3e8:rt:t6:t9 0x3ec:rt:t8:t7 0x3f0:rs:t7:t6 0x3f0:rt:t9:t8 0x3f4:rt:t7:t6 0x3f8:rt:t7:t6 0x408:rs:t6:t9 0x408:rt:t8:t7 0x424:rt:v0:t8 0x42c:rs:v0:t8 0x42c:rt:v0:t9 0x430:rs:v0:t9 0x430:rt:t9:t7 0x434:rs:t9:t7 0x43c:rt:v0:t9 0x444:rt:v0:t6 0x44c:rs:v0:t6 0x44c:rt:v0:t8 0x450:rs:v0:t8 0x450:rt:t6:t9 0x454:rs:t6:t9 0x45c:rt:v0:t8 0x464:rt:v0:t7 0x46c:rs:v0:t7 0x46c:rt:v0:t6 0x470:rs:v0:t6 0x47c:rt:v0:t6 0x484:rt:t7:t9 0x48c:rs:t7:t9 0x4a8:rt:t4:a3 0x4b0:rt:t4:a3 0x4b8:rt:v0:v1 0x4c4:rt:v1:v0 0x4cc:rs:v0:v1 0x4cc:rt:v0:v1 0x4d0:rs:v0:v1 0x4d0:rt:v1:v0 0x4d4:rs:v0:v1 0x4d8:rs:v0:v1 0x4e0:rs:v0:v1 0x4e4:rt:t9:t7 0x4f0:rt:t9:t7 0x530:rt:v0:a1 0x53c:rs:v0:a1 0x53c:rt:v0:a1 0x544:rs:v0:a1 0x548:rt:t7:t9 0x558:rt:t7:t9 0x55c:rt:t9:t7 0x564:rs:t9:t7 */
/* tt_serve_step - TT overlay, Address: 0x801400A0, Size: 1396 bytes (349 insns) */
#include "types.h"

extern u8 tt_anim_tbl[];
extern u8 tt_script_seq_tbl[];
extern u8 *tt_player_wk;
extern u8 *tt_boss_wk;

extern void tt_cmd_once(s32 id);
extern s32 tt_angle_between(void *a, void *b);
extern void tt_shot_heading_make(s32 x, s32 y, s32 a, s32 b, s32 c);
extern void tt_player_anim(void);
extern void tt_seq_step(u8 *tbl, s32 idx);

void tt_serve_step(void)
{
    u8 *p;
    u8 *b;
    u8 *c;
    u8 *d;
    u8 *e;
    s32 i;
    s32 n;
    s32 t;
    s32 ang;
    s32 tmp[9];

    p = tt_boss_wk;
    b = tt_player_wk;
    switch ((u32)*(u16 *)(p + 8)) {
    case 0:
        tt_cmd_once(1298);
        *(u16 *)(p + 870) = 0;
        c = p + 320;
        d = p + 16;
        e = p + 32;
        for (i = 320; i != 380; i += 20) {
            *(u8 **)(c + 56) = tt_anim_tbl;
            *(u8 *)(c + 60) = 0;
            *(u8 *)(c + 61) = 10;
            *(u8 *)(c + 62) = 16;
            *(u8 *)(c + 64) = 96;
            *(u8 *)(c + 67) = 15;
            *(u8 *)(c + 72) = 0;
            *(u8 *)(d + 784) = 1;
            c += 20;
            d += 1;
            e += 2;
            *(s16 *)(e + 678) = 7;
            *(s16 *)(e + 582) = 0;
            *(s16 *)(e + 630) = 8;
        }
        *(s32 *)(p + 1004) = 0x00080000;
        *(s32 *)(p + 1008) = 0xFFFF0000;
        *(s32 *)(p + 1012) = 1024;
        *(s32 *)(p + 1016) = 0;
        *(u8 *)(p + 835) = 0;
        *(u16 *)(p + 8) = 64;
        break;
    case 64:
        if (*(s32 *)(tt_boss_wk + 1004) >= 0x00200001) {
            *(s32 *)(p + 1012) = -1024;
        } else {
            *(s32 *)(p + 1012) = 1024;
        }
        e = p + 32;
        if (*(s32 *)(p + 1004) >= 0x00200001 &&
            *(s32 *)(p + 1008) <= 0) {
            *(s32 *)(p + 1012) = 0;
            *(u16 *)(p + 8) = 80;
        }
        n = *(s32 *)(p + 1016) + 16384;
        *(s32 *)(p + 1004) = *(s32 *)(p + 1004) + *(s32 *)(p + 1008);
        *(s32 *)(p + 1016) = n;
        *(s32 *)(p + 1008) = *(s32 *)(p + 1008) + *(s32 *)(p + 1012);
        *(s16 *)(p + 618) = (s16)(-n / 65536);
        *(s16 *)(p + 620) = (s16)(n / 65536);
        for (i = 16; i != 19; i++) {
            t = *(s32 *)(p + 1004) / 65536;
            e += 2;
            *(s16 *)(e + 630) = (s16)t;
        }
        break;
    case 80:
        *(u16 *)(p + 870) = 0;
        *(u16 *)(p + 8) = 81;
        break;
    case 81:
        if ((u32)*(u16 *)(tt_boss_wk + 870) >= 32U) {
            *(u16 *)(p + 870) = 0;
            *(u16 *)(p + 8) = 82;
        }
        break;
    case 82:
        *(u16 *)(p + 870) = 0;
        *(u16 *)(p + 980) = 0;
        *(u16 *)(p + 982) = 1;
        *(u16 *)(p + 984) = 2;
        *(u16 *)(p + 8) = 83;
        break;
    case 83:
        c = tt_boss_wk;
        if (*(u8 *)(c + 382) == 18 && (*(u16 *)(c + 870) & 7) == 0) {
            tt_cmd_once(1299);
        }
        if (*(u8 *)(p + 382) == 18 && (*(u16 *)(p + 870) & 7) == 7) {
            tmp[3] = (*(s16 *)(p + 20) +
                      *(s16 *)(p + 616 + 2 * *(u16 *)(p + 980))) << 16;
            tmp[4] = (*(s16 *)(p + 22) +
                      *(s16 *)(p + 664 + 2 * *(u16 *)(p + 980))) << 16;
            ang = tt_angle_between(tmp, b + 20);
            for (i = 0; i != 4096; i += 1024) {
                tt_shot_heading_make(*(s16 *)(p + 20) +
                              *(s16 *)(p + 616 + 2 * *(u16 *)(p + 980)),
                              *(s16 *)(p + 22) +
                              *(s16 *)(p + 664 + 2 * *(u16 *)(p + 980)),
                              24, ang + i, 160);
                tt_shot_heading_make(*(s16 *)(p + 20) +
                              *(s16 *)(p + 616 + 2 * *(u16 *)(p + 982)),
                              *(s16 *)(p + 22) +
                              *(s16 *)(p + 664 + 2 * *(u16 *)(p + 982)),
                              24, i, 160);
                tt_shot_heading_make(*(s16 *)(p + 20) +
                              *(s16 *)(p + 616 + 2 * *(u16 *)(p + 984)),
                              *(s16 *)(p + 22) +
                              *(s16 *)(p + 664 + 2 * *(u16 *)(p + 984)),
                              24, i + 512, 160);
            }
        }
        tt_player_anim();
        t = *(u16 *)(p + 980) + 1;
        *(u16 *)(p + 980) = (s16)t;
        if ((u16)t >= 3U) {
            *(u16 *)(p + 980) = 0;
        }
        t = *(u16 *)(p + 982) + 1;
        *(u16 *)(p + 982) = (s16)t;
        if ((u16)t >= 3U) {
            *(u16 *)(p + 982) = 0;
        }
        t = *(u16 *)(p + 984) + 1;
        *(u16 *)(p + 984) = (s16)t;
        if ((u16)t >= 3U) {
            *(u16 *)(p + 984) = 0;
        }
        if ((u32)*(u16 *)(p + 870) >= 384U) {
            *(u16 *)(p + 8) = 128;
        }
        break;
    case 128:
        tt_cmd_once(1290);
        *(u8 *)(p + 835) = 1;
        c = p + 320;
        for (i = 16; i != 19; i++) {
            c += 20;
            *(u8 *)(c + 41) = 4;
            *(u8 *)(c + 42) = 52;
            *(u8 *)(c + 40) = 0;
            *(u8 *)(c + 44) = 128;
        }
        *(u16 *)(p + 870) = 0;
        *(u16 *)(p + 8) = 129;
        break;
    case 129:
        for (i = 16; i != 19; i++) {
            tt_seq_step(tt_script_seq_tbl, i);
        }
        if ((u32)*(u16 *)(p + 870) >= 64U) {
            *(u16 *)(p + 8) = 160;
        }
        break;
    case 160:
        for (i = 16; i != 19; i++) {
            *(u8 *)(p + 784 + i) = 0;
        }
        *(s32 *)(p + 1000) = 0;
        *(s16 *)(p + 6) = 0;
        *(u16 *)(p + 8) = 0;
        *(s32 *)(p + 996) = *(s32 *)(p + 32);
        break;
    }
    *(u16 *)(p + 870) = (s16)(*(u16 *)(p + 870) + 1);
}
