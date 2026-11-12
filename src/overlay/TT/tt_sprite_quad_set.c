/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: remap_stack_slots --map '92:84,88:80,84:76,80:72,56:60,42:54,40:52,38:50,36:48' */
/* @PostProc: swap_instructions --offset1 0xe0 --offset2 0xe4 */
/* @PostProc: swap_instructions --offset1 0x148 --offset2 0x14c */
/* @PostProc: swap_instructions --offset1 0x220 --offset2 0x234 */
/* @PostProc: swap_instructions --offset1 0x224 --offset2 0x234 */
/* @PostProc: swap_instructions --offset1 0x228 --offset2 0x234 */
/* @PostProc: swap_instructions --offset1 0x22c --offset2 0x234 */
/* @PostProc: swap_instructions --offset1 0x230 --offset2 0x234 */
/* @PostProc: swap_instructions --offset1 0x244 --offset2 0x258 */
/* @PostProc: swap_instructions --offset1 0x248 --offset2 0x258 */
/* @PostProc: swap_instructions --offset1 0x24c --offset2 0x258 */
/* @PostProc: swap_instructions --offset1 0x250 --offset2 0x258 */
/* @PostProc: swap_instructions --offset1 0x254 --offset2 0x258 */
/* @PostProc: swap_instructions --offset1 0x268 --offset2 0x27c */
/* @PostProc: swap_instructions --offset1 0x26c --offset2 0x27c */
/* @PostProc: swap_instructions --offset1 0x270 --offset2 0x27c */
/* @PostProc: swap_instructions --offset1 0x274 --offset2 0x27c */
/* @PostProc: swap_instructions --offset1 0x278 --offset2 0x27c */
/* @PostProc: swap_instructions --offset1 0x2ac --offset2 0x2c0 */
/* @PostProc: swap_instructions --offset1 0x2b0 --offset2 0x2c0 */
/* @PostProc: swap_instructions --offset1 0x2b4 --offset2 0x2c0 */
/* @PostProc: swap_instructions --offset1 0x2b8 --offset2 0x2c0 */
/* @PostProc: swap_instructions --offset1 0x2bc --offset2 0x2c0 */
/* @PostProc: swap_instructions --offset1 0x2d0 --offset2 0x2e4 */
/* @PostProc: swap_instructions --offset1 0x2d4 --offset2 0x2e4 */
/* @PostProc: swap_instructions --offset1 0x2d8 --offset2 0x2e4 */
/* @PostProc: swap_instructions --offset1 0x2dc --offset2 0x2e4 */
/* @PostProc: swap_instructions --offset1 0x2e0 --offset2 0x2e4 */
/* @PostProc: swap_instructions --offset1 0x2f4 --offset2 0x308 */
/* @PostProc: swap_instructions --offset1 0x2f8 --offset2 0x308 */
/* @PostProc: swap_instructions --offset1 0x2fc --offset2 0x308 */
/* @PostProc: swap_instructions --offset1 0x300 --offset2 0x308 */
/* @PostProc: swap_instructions --offset1 0x304 --offset2 0x308 */
/* @PostProc: swap_instructions --offset1 0x598 --offset2 0x5a4 */
/* @PostProc: swap_instructions --offset1 0x5b0 --offset2 0x5c4 */
/* @PostProc: swap_instructions --offset1 0x5b4 --offset2 0x5bc */
/* @PostProc: swap_instructions --offset1 0x5b8 --offset2 0x5c0 */
/* @PostProc: swap_instructions --offset1 0x5bc --offset2 0x5c4 */
/* @PostProc: swap_instructions --offset1 0x5c0 --offset2 0x5c4 */
/* @PostProc: patch_insn_reg_field 0x20:rt:t0:t1 0x24:rt:t1:t0 0x98:rt:t8:v0 0xa0:rt:t8:v0 0xa4:rt:t7:v1 0xa8:rd:t6:t8 0xa8:rs:t8:v0 0xa8:rt:t1:t0 0xac:rs:t7:v1 0xac:rt:t0:t1 0xb0:rs:t6:t8 0xbc:rt:t7:v1 0xc0:rt:t6:v0 0xc8:rt:t6:v0 0xd0:rs:t6:v0 0xd0:rt:t1:t0 0xd4:rd:t9:t6 0xd4:rt:t0:t1 0xd8:rs:t9:t6 0xd8:rt:t6:t7 0xe0:rt:t6:t7 0xf8:rd:t7:t9 0xf8:rt:t1:t0 0xfc:rs:t7:t9 0xfc:rt:t8:t6 0x100:rt:t8:t6 0x104:rt:t9:v1 0x10c:rs:t9:v1 0x10c:rt:t0:t1 0x11c:rt:t9:v1 0x128:rt:t1:t0 0x13c:rt:t0:t1 0x164:rs:t1:t0 0x164:rt:t1:t0 0x174:rs:t1:t0 0x174:rt:t1:t0 0x188:rs:t0:t1 0x188:rt:t0:t1 0x198:rs:t0:t1 0x198:rt:t0:t1 0x1b8:rt:t8:t6 0x1c4:rt:t1:t0 0x1c8:rt:t0:t1 0x1f0:rt:t1:t0 0x1f4:rt:t0:t1 0x214:rt:t8:t9 0x218:rd:t9:t8 0x220:rt:t6:t9 0x22c:rs:t7:v0 0x22c:rt:v0:t7 0x238:rt:t9:t7 0x23c:rd:t7:t6 0x23c:rs:t6:t9 0x240:rs:t9:t6 0x244:rt:t8:t7 0x24c:rd:t6:t9 0x250:rd:t9:t6 0x250:rs:t6:v0 0x250:rt:v0:t9 0x254:rt:t9:t6 0x25c:rt:t7:t9 0x260:rd:t6:t8 0x260:rs:t8:t7 0x264:rs:t7:t8 0x264:rt:t6:t9 0x26c:rd:t9:t6 0x270:rd:t8:t7 0x270:rt:t9:t6 0x274:rd:t7:t8 0x274:rs:t8:v0 0x274:rt:v0:t7 0x278:rt:t7:t8 0x280:rt:t6:t7 0x284:rd:t8:t6 0x288:rt:t8:t7 0x28c:rd:t7:t8 0x290:rt:t7:t8 0x294:rs:t9:v0 0x294:rt:v0:t9 0x29c:rt:v0:v1 0x2a4:rt:v0:v1 0x2a8:rs:t8:t7 0x2a8:rt:t7:t8 0x2ac:rt:t9:t8 0x2b8:rd:t8:t7 0x2b8:rs:t6:v1 0x2b8:rt:v0:t6 0x2bc:rt:t8:t7 0x2c0:rt:v0:v1 0x2c4:rt:t7:t6 0x2c8:rd:t6:t9 0x2c8:rs:t9:t8 0x2c8:rt:v0:v1 0x2cc:rs:t7:t9 0x2d0:rt:t8:t6 0x2d4:rd:t8:t7 0x2d8:rd:t9:t8 0x2d8:rt:t8:t7 0x2dc:rd:t7:t9 0x2dc:rs:t9:v1 0x2dc:rt:v0:t8 0x2e0:rt:t7:t9 0x2e4:rt:v0:v1 0x2e8:rt:t6:t8 0x2ec:rd:t9:t7 0x2ec:rs:t8:t6 0x2ec:rt:v0:v1 0x2f0:rs:t6:t7 0x2f0:rt:t9:t8 0x2f4:rt:t7:t8 0x2f8:rd:t7:t9 0x2fc:rd:t8:t6 0x2fc:rt:t7:t9 0x300:rd:t6:t7 0x300:rs:t8:v1 0x300:rt:v0:t6 0x304:rt:t6:t7 0x308:rt:v0:v1 0x30c:rt:t9:t6 0x310:rd:t8:t9 0x310:rs:t7:t8 0x310:rt:v0:v1 0x314:rt:t8:t6 0x318:rd:t6:t7 0x31c:rd:t7:t8 0x31c:rt:t6:t7 0x320:rs:t7:v1 0x320:rt:v0:t8 0x328:rt:t8:t6 0x330:rs:t8:t6 0x334:rt:t6:t7 0x360:rt:t6:t7 0x368:rs:t6:t7 0x374:rt:t7:t8 0x37c:rs:t7:t8 0x394:rd:t8:t6 0x398:rd:t6:t7 0x398:rt:t8:t6 0x39c:rd:t7:t8 0x39c:rt:t6:t7 0x3a0:rt:t7:t8 0x3a8:rt:t0:a3 0x3ac:rt:a3:t0 0x3b0:rd:t8:t6 0x3b0:rs:t0:a3 0x3b4:rs:t8:t6 0x3b8:rd:t7:t8 0x3b8:rs:a3:t0 0x3bc:rd:t6:t7 0x3c8:rs:t7:t8 0x3d0:rd:t8:t6 0x3d0:rs:t6:t7 0x3d4:rd:t7:t8 0x3d4:rt:t8:t6 0x3d8:rd:t6:t7 0x3d8:rt:t7:t8 0x3dc:rt:t6:t7 0x3f0:rd:t7:t8 0x3f4:rd:t8:t6 0x400:rs:t7:t8 0x404:rd:t6:t7 0x408:rs:t8:t6 0x408:rt:t6:t7 0x40c:rd:t7:t8 0x410:rd:t8:t6 0x410:rt:t7:t8 0x414:rt:t8:t6 0x418:rt:t6:t7 0x41c:rt:t8:t6 0x420:rs:t6:t7 0x428:rd:t6:t7 0x428:rs:t8:t6 0x42c:rd:t7:t8 0x438:rs:t6:t7 0x440:rd:t8:t6 0x440:rs:t7:t8 0x444:rd:t6:t7 0x444:rt:t8:t6 0x44c:rd:t7:t8 0x44c:rt:t6:t7 0x450:rt:t7:t8 0x464:rd:t8:t6 0x468:rd:t6:t7 0x468:rt:t8:t6 0x46c:rd:t7:t8 0x46c:rt:t6:t7 0x470:rt:t7:t8 0x478:rt:t8:t6 0x480:rd:t6:t7 0x480:rs:t0:a3 0x480:rt:t8:t6 0x484:rs:t6:t7 0x488:rs:a3:t0 0x48c:rd:t7:t8 0x49c:rd:t8:t6 0x4a0:rd:t6:t7 0x4a0:rs:t7:t8 0x4a0:rt:t8:t6 0x4a4:rt:t6:t7 0x4a8:rd:t7:t8 0x4ac:rt:t7:t8 0x4b0:rt:t8:t6 0x4b8:rd:t6:t7 0x4b8:rt:t8:t6 0x4bc:rs:t6:t7 0x4c0:rd:t7:t8 0x4d0:rs:t7:t8 0x4d4:rd:t8:t6 0x4d8:rd:t6:t7 0x4d8:rt:t8:t6 0x4dc:rd:t7:t8 0x4dc:rt:t6:t7 0x4e0:rt:t7:t8 0x4e8:rt:t6:t7 0x4ec:rt:t8:t6 0x4f4:rd:t7:t8 0x4f4:rs:t8:t6 0x4f4:rt:t6:t7 0x4f8:rs:t7:t8 0x4fc:rt:t8:t6 0x504:rd:t6:t7 0x504:rs:t8:t6 0x514:rs:t6:t7 0x518:rd:t7:t8 0x51c:rd:t8:t6 0x51c:rt:t7:t8 0x520:rd:t6:t7 0x520:rt:t8:t6 0x524:rt:t6:t7 0x52c:rt:t7:t8 0x534:rt:t7:t8 0x538:rt:t8:t6 0x540:rt:t8:t6 0x544:rt:t6:t7 0x54c:rt:t6:t7 0x55c:rt:t7:t8 0x560:rt:t8:t6 0x564:rs:t7:t8 0x57c:rs:t8:t6 0x594:rt:t6:t7 0x59c:rt:t6:t7 0x5ac:rt:t7:t8 0x5b4:rt:t7:t8 0x5b8:rt:t8:t6 0x5c8:rt:t8:t6 */
/* tt_sprite_quad_set - TT overlay, Address: 0x80141440, Size: 1484 bytes (371 insns) */
#include "types.h"

extern s32 rsin(s32 angle);
extern s32 rcos(s32 angle);

void tt_sprite_quad_set(u8 *q, u8 *s)
{
    s32 u;
    s32 v;
    s32 ex;
    s32 ey;
    s32 w;
    s32 h;
    s32 dx;
    s32 dy;
    s32 c;
    s32 sn;
    s32 cs;
    s32 x0;
    s32 y0;
    s16 n0;
    s16 n1;
    s16 n2;
    s16 n3;

    h = *(s16 *)(q + 18);
    w = *(s16 *)(q + 16);
    if (*(s16 *)(s + 0) >= 8192) {
        dx = -1;
    } else {
        dx = 0;
    }
    if (*(s16 *)(s + 2) >= 8192) {
        dy = -1;
    } else {
        dy = 0;
    }
    switch (*(u8 *)(q + 40)) {
    case 0:
        u = *(u8 *)(q + 12);
        ex = u + w + dx;
        v = *(u8 *)(q + 13);
        ey = v + h + dy;
        break;
    case 1:
        u = *(u8 *)(q + 12);
        ex = u + w + dx;
        v = *(u8 *)(q + 13) + h - 1;
        ey = *(u8 *)(q + 13) - dy - 1;
        break;
    case 2:
        u = *(u8 *)(q + 12) + w - 1;
        ex = *(u8 *)(q + 12) - dx - 1;
        v = *(u8 *)(q + 13);
        ey = v + h + dy;
        break;
    case 3:
        u = *(u8 *)(q + 12) + w - 1;
        ex = *(u8 *)(q + 12) - dx - 1;
        v = *(u8 *)(q + 13) + h - 1;
        ey = *(u8 *)(q + 13) - dy - 1;
        break;
    }
    if (ex < 0) {
        ex = 0;
        w = w - 1;
    } else if (ex >= 256) {
        ex = 255;
        w = w - 1;
    }
    if (ey < 0) {
        ey = 0;
        h = h - 1;
    } else if (ey >= 256) {
        ey = 255;
        h = h - 1;
    }
    switch (*(u8 *)(q + 43)) {
    case 0:
        x0 = *(s16 *)(q + 8);
        y0 = *(s16 *)(q + 10);
        *(s16 *)(q + 16) = (s16)(x0 + w);
        *(s16 *)(q + 26) = (s16)(y0 + h);
        *(s16 *)(q + 32) = (s16)(x0 + w);
        *(s16 *)(q + 34) = (s16)(y0 + h);
        *(s16 *)(q + 24) = (s16)x0;
        *(s16 *)(q + 18) = (s16)y0;
        break;
    case 1:
        x0 = *(s16 *)(q + 8);
        y0 = *(s16 *)(q + 10);
        *(s16 *)(q + 16) = (s16)(x0 + w);
        *(s16 *)(q + 26) = (s16)(y0 + h);
        *(s16 *)(q + 32) = (s16)(x0 + w);
        *(s16 *)(q + 34) = (s16)(y0 + h);
        *(s16 *)(q + 24) = (s16)x0;
        *(s16 *)(q + 18) = (s16)y0;
        c = *(s16 *)(s + 4);
        *(s16 *)(q + 8) =
            (s16)((((x0 - c) * *(s16 *)(s + 0)) >> 12) + c);
        c = *(s16 *)(s + 4);
        *(s16 *)(q + 16) =
            (s16)((((*(s16 *)(q + 16) - c) * *(s16 *)(s + 0)) >> 12) + c);
        c = *(s16 *)(s + 4);
        *(s16 *)(q + 24) =
            (s16)((((*(s16 *)(q + 24) - c) * *(s16 *)(s + 0)) >> 12) + c);
        c = *(s16 *)(s + 4);
        *(s16 *)(q + 32) =
            (s16)((((*(s16 *)(q + 32) - c) * *(s16 *)(s + 0)) >> 12) + c);
        c = *(s16 *)(s + 6);
        *(s16 *)(q + 10) =
            (s16)((((y0 - c) * *(s16 *)(s + 2)) >> 12) + c);
        c = *(s16 *)(s + 6);
        *(s16 *)(q + 18) =
            (s16)((((*(s16 *)(q + 18) - c) * *(s16 *)(s + 2)) >> 12) + c);
        c = *(s16 *)(s + 6);
        *(s16 *)(q + 26) =
            (s16)((((*(s16 *)(q + 26) - c) * *(s16 *)(s + 2)) >> 12) + c);
        c = *(s16 *)(s + 6);
        *(s16 *)(q + 34) =
            (s16)((((*(s16 *)(q + 34) - c) * *(s16 *)(s + 2)) >> 12) + c);
        break;
    }
    if (*(u8 *)(q + 44) != 0) {
        sn = rcos(*(s32 *)(s + 12));
        cs = rsin(*(s32 *)(s + 12));
        n0 = (s16)((((*(s16 *)(q + 8) - *(s16 *)(s + 8)) * sn -
               (*(s16 *)(q + 10) - *(s16 *)(s + 10)) * cs) >> 12) +
             *(s16 *)(s + 8));
        n1 = (s16)((((*(s16 *)(q + 16) - *(s16 *)(s + 8)) * sn -
               (*(s16 *)(q + 18) - *(s16 *)(s + 10)) * cs) >> 12) +
             *(s16 *)(s + 8));
        n2 = (s16)((((*(s16 *)(q + 24) - *(s16 *)(s + 8)) * sn -
               (*(s16 *)(q + 26) - *(s16 *)(s + 10)) * cs) >> 12) +
             *(s16 *)(s + 8));
        n3 = (s16)((((*(s16 *)(q + 32) - *(s16 *)(s + 8)) * sn -
               (*(s16 *)(q + 34) - *(s16 *)(s + 10)) * cs) >> 12) +
             *(s16 *)(s + 8));
        *(s16 *)(q + 10) = (s16)((((*(s16 *)(q + 8) - *(s16 *)(s + 8)) * cs +
               (*(s16 *)(q + 10) - *(s16 *)(s + 10)) * sn) >> 12) +
             *(s16 *)(s + 10));
        *(s16 *)(q + 18) = (s16)((((*(s16 *)(q + 16) - *(s16 *)(s + 8)) * cs +
               (*(s16 *)(q + 18) - *(s16 *)(s + 10)) * sn) >> 12) +
             *(s16 *)(s + 10));
        *(s16 *)(q + 26) = (s16)((((*(s16 *)(q + 24) - *(s16 *)(s + 8)) * cs +
               (*(s16 *)(q + 26) - *(s16 *)(s + 10)) * sn) >> 12) +
             *(s16 *)(s + 10));
        *(s16 *)(q + 34) = (s16)((((*(s16 *)(q + 32) - *(s16 *)(s + 8)) * cs +
               (*(s16 *)(q + 34) - *(s16 *)(s + 10)) * sn) >> 12) +
             *(s16 *)(s + 10));
        *(s16 *)(q + 8) = n0;
        *(s16 *)(q + 16) = n1;
        *(s16 *)(q + 24) = n2;
        *(s16 *)(q + 32) = n3;
        if (u < ex) {
            ex = ex - 1;
        } else {
            ex = ex + 1;
        }
        if (v < ey) {
            ey = ey - 1;
        } else {
            ey = ey + 1;
        }
    }
    *(u8 *)(q + 12) = (u8)u;
    *(u8 *)(q + 20) = (u8)ex;
    *(u8 *)(q + 13) = (u8)v;
    *(u8 *)(q + 21) = (u8)v;
    *(u8 *)(q + 37) = (u8)ey;
    *(u8 *)(q + 36) = (u8)ex;
    *(u8 *)(q + 29) = (u8)ey;
    *(u8 *)(q + 28) = (u8)u;
}
