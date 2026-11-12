/* @RodataBase: 0x8014D4B0 */
/* @PostProc: patch_insn_reg_field 0x24:rt:v0:t7 0x2c:rs:v0:t7 0x34:rt:v0:t7 0x50:rt:v1:v0 0x54:rt:a1:a0 0x58:rt:a0:a1 0x5c:rt:a1:a0 0x60:rt:a1:a0 0x68:rt:a0:a1 0x74:rt:a0:a1 0x78:rt:a0:a1 0x7c:rt:v1:v0 0x80:rt:v1:v0 0x84:rt:v1:v0 0x88:rt:v1:v0 0x8c:rd:v0:v1 0x90:rs:v0:v1 0x90:rt:a0:a1 0x94:rs:v0:v1 0x94:rt:v1:v0 0x98:rs:v0:v1 0x98:rt:v1:v0 0x9c:rs:v0:v1 0x9c:rt:a0:a1 0xa0:rs:v0:v1 0xa0:rt:v1:v0 0xa4:rs:v0:v1 0xa4:rt:v1:v0 0xa8:rs:v0:v1 0xa8:rt:a0:a1 0xac:rs:v0:v1 0xac:rt:v1:v0 0xb0:rs:v0:v1 0xb0:rt:v1:v0 0xb4:rs:v0:v1 0xb4:rt:a0:a1 0xb8:rs:v0:v1 0xb8:rt:v1:v0 0xbc:rs:v0:v1 0xbc:rt:v1:v0 0xc4:rt:a0:a1 0xdc:rd:a1:a0 0xe0:rd:a0:v1 0xf4:rd:v1:a1 0xf8:rs:a1:a0 0xf8:rt:a1:a0 0xfc:rs:a1:a0 0x100:rs:a0:v1 0x100:rt:a0:v1 0x104:rs:a0:v1 0x104:rt:v1:a1 0x10c:rs:a0:v1 0x10c:rt:v1:a1 0x110:rt:v1:t1 0x118:rs:v1:t1 0x118:rt:v0:t2 0x11c:rs:v0:t2 0x11c:rt:t1:t3 0x120:rs:t1:t3 0x128:rt:v0:t2 0x138:rt:t2:t4 0x13c:rt:t2:t4 0x140:rt:t3:t5 0x148:rs:t3:t5 0x150:rt:t4:t6 0x158:rt:t4:t6 0x15c:rt:t5:t7 0x160:rt:t5:t7 0x164:rd:t6:t8 0x170:rt:t6:t8 0x184:rt:t7:t0 0x18c:rt:t7:t0 0x1b0:rd:t8:t9 0x1b4:rt:t8:t9 0x1cc:rd:t0:t1 0x1d0:rs:t0:t1 0x1d0:rt:t9:t2 0x1d4:rt:t9:t2 0x1d8:rt:t1:t3 0x1e0:rs:t1:t3 0x1e8:rt:v0:t4 0x1ec:rt:t3:t7 0x1f0:rs:v0:t4 0x1f0:rt:v0:t5 0x1f4:rs:v0:t5 0x1f4:rt:t2:t6 0x1f8:rs:t2:t6 0x200:rt:v0:t5 0x204:rt:t4:t8 0x208:rt:t3:t7 0x218:rt:t4:t8 0x21c:rt:v0:t0 0x220:rt:t6:t2 0x224:rs:v0:t0 0x224:rt:v0:t9 0x228:rs:v0:t9 0x228:rt:t5:t1 0x22c:rs:t5:t1 0x234:rt:v0:t9 0x238:rt:t7:t3 0x23c:rt:t6:t2 0x24c:rt:t7:t3 0x260:rt:t0:t5 0x264:rd:a1:a0 0x268:rd:a0:v1 0x270:rd:t8:t4 0x278:rd:t8:t4 0x27c:rd:v1:a1 0x27c:rs:t0:t5 0x27c:rt:t8:t4 0x280:rs:a1:a0 0x280:rt:a1:a0 0x284:rs:a1:a0 0x288:rs:a0:v1 0x288:rt:a0:v1 0x28c:rs:a0:v1 0x28c:rt:v1:a1 0x294:rs:a0:v1 0x294:rt:v1:a1 0x298:rt:v1:t6 0x29c:rt:t1:t0 0x2a0:rs:v1:t6 0x2a0:rt:v0:t7 0x2a4:rs:v0:t7 0x2a4:rt:t9:t8 0x2a8:rs:t9:t8 0x2b0:rt:v0:t7 0x2bc:rd:t2:t9 0x2c0:rt:t1:t0 0x2d0:rt:t2:t9 0x2e4:rt:t3:t1 0x2ec:rt:t3:t1 0x2f0:rt:v1:a0 0x2f4:rd:t4:t2 0x2f4:rt:v1:a0 0x2f8:rd:t4:t2 0x2f8:rs:t4:t2 0x2f8:rt:v1:a0 0x2fc:rd:t4:t2 0x2fc:rt:t4:t2 0x308:rd:v0:v1 0x308:rt:t4:t2 0x30c:rs:v0:v1 0x310:rs:v0:v1 0x314:rs:v0:v1 0x318:rs:v0:v1 0x31c:rt:a1:t3 0x320:rt:a0:a1 0x324:rs:a1:t3 0x32c:rt:a0:a1 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:7,lod:37,lod:59,lod:64 IDO52_EXCLUDE_REGS_WEB=0:s0+s1+s2+s3+s4+s5+s6+s7+s8+a2 IDO52_NO_LOOP_TEST_REPLACEMENT=1 */
#include "types.h"

extern u8 *tt_boss_wk;

extern s32 rsin();
extern s32 rcos();
extern s32 tt_cmd_once();

void tt_orbit_turn_step(u8 flag)
{
    u8 *a3;
    s32 sel;

    a3 = tt_boss_wk;
    sel = *(u16 *)(a3 + 8);

    switch ((u32)sel) {
    case 0:
    {
        u8 *v1;
        s32 v0;
        s32 k;

        v0 = 4096;
        k = 2;
        *(s8 *)(a3 + 834) = 1;
        *(s8 *)(a3 + 835) = 0;
        *(s16 *)(a3 + 944) = 0;
        *(s8 *)(a3 + 72) = 1;
        *(s8 *)(a3 + 92) = 1;
        *(s16 *)(a3 + 68) = (s16)v0;
        *(s16 *)(a3 + 70) = (s16)v0;
        *(s16 *)(a3 + 88) = (s16)v0;
        *(s16 *)(a3 + 90) = (s16)v0;

        v1 = (u8 *)((s32)a3 + k * 20);
        *(s8 *)(v1 + 92) = 1;
        *(s16 *)(v1 + 88) = (s16)v0;
        *(s16 *)(v1 + 90) = (s16)v0;
        *(s8 *)(v1 + 112) = 1;
        *(s16 *)(v1 + 108) = (s16)v0;
        *(s16 *)(v1 + 110) = (s16)v0;
        *(s8 *)(v1 + 132) = 1;
        *(s16 *)(v1 + 128) = (s16)v0;
        *(s16 *)(v1 + 130) = (s16)v0;
        *(s8 *)(v1 + 72) = 1;
        *(s16 *)(v1 + 68) = (s16)v0;
        *(s16 *)(v1 + 70) = (s16)v0;

        *(s16 *)(a3 + 8) = 1;
        break;
    }
    case 1:
    {
        s32 v0;
        s32 a1;
        s32 i;
        u8 *v1;
        s32 t1;
        s32 t2;
        u32 t3;
        u8 *v0p;
        s32 k;

        v0 = rsin(*(u16 *)(a3 + 944));
        a1 = 4096 - v0 / 2;

        v1 = a3;
        for (i = 0; i < 6; i++) {
            v1 += 20;
            *(s16 *)(v1 + 50) = (s16)a1;
            *(s16 *)(v1 + 48) = (s16)a1;
        }

        t1 = *(u16 *)(a3 + 944);
        t2 = t1 + 16;
        *(s16 *)(a3 + 944) = (s16)t2;
        t3 = (u32)t2 & 0xFFFF;
        if (t3 < 1024) {
            break;
        }

        tt_cmd_once(1297);
        *(s16 *)(a3 + 944) = 1024;

        if (flag != 0) {
            *(s16 *)(a3 + 946) = 7168;
        } else {
            *(s16 *)(a3 + 946) = 3072;
        }

        k = 2;
        *(s16 *)(a3 + 680) = 10;
        *(s16 *)(a3 + 682) = 10;
        v0p = (u8 *)((s32)a3 + k * 2);
        *(s16 *)(v0p + 682) = 10;
        *(s16 *)(v0p + 684) = 10;
        *(s16 *)(v0p + 686) = 10;
        *(s16 *)(v0p + 680) = 10;

        *(s16 *)(a3 + 8) = 2;
        break;
    }
    case 2:
    {
        s32 v0;
        s32 t5;
        u32 t6;
        s32 t9;
        u32 t1;

        v0 = rcos(*(u16 *)(a3 + 946));
        *(s16 *)(a3 + 948) = (s16)(v0 / 40);

        v0 = rsin(*(u16 *)(a3 + 946));
        *(s16 *)(a3 + 950) = (s16)(v0 / 51 + 80);

        if (flag != 0) {
            t5 = *(u16 *)(a3 + 946) - 32;
            *(s16 *)(a3 + 946) = (s16)t5;
            t6 = (u32)t5 & 0xFFFF;
            if (t6 < 3073) {
                *(s16 *)(a3 + 946) = 3072;
                *(s16 *)(a3 + 948) = 0;
                *(s16 *)(a3 + 950) = 0;
                *(s16 *)(a3 + 8) = 3;
            }
        } else {
            t9 = *(u16 *)(a3 + 946) + 32;
            *(s16 *)(a3 + 946) = (s16)t9;
            t1 = (u32)t9 & 0xFFFF;
            if (t1 >= 7168) {
                *(s16 *)(a3 + 946) = 7168;
                *(s16 *)(a3 + 948) = 0;
                *(s16 *)(a3 + 950) = 0;
                *(s16 *)(a3 + 8) = 3;
            }
        }
        break;
    }
    case 3:
    {
        s32 v0;
        s32 a1;
        s32 i;
        u8 *v1;
        s32 t6;
        s32 t7;
        u32 t8;
        u8 *v0p;
        s32 k;

        v0 = rsin(*(u16 *)(a3 + 944));
        a1 = 4096 - v0 / 2;

        v1 = a3;
        for (i = 0; i < 6; i++) {
            v1 += 20;
            *(s16 *)(v1 + 50) = (s16)a1;
            *(s16 *)(v1 + 48) = (s16)a1;
        }

        t6 = *(u16 *)(a3 + 944);
        t7 = t6 + 32;
        *(s16 *)(a3 + 944) = (s16)t7;
        t8 = (u32)t7 & 0xFFFF;
        if (t8 < 2048) {
            break;
        }

        *(s16 *)(a3 + 944) = 2048;
        *(s16 *)(a3 + 946) = 0;
        *(s16 *)(a3 + 680) = 8;
        *(s16 *)(a3 + 682) = 8;
        k = 2;
        v0p = (u8 *)((s32)a3 + k * 2);
        *(s16 *)(v0p + 682) = 8;
        *(s16 *)(v0p + 684) = 8;
        *(s16 *)(v0p + 686) = 8;
        *(s16 *)(v0p + 680) = 8;

        *(s16 *)(a3 + 8) = 4;
        break;
    }
    case 4:
    {
        u8 *v1;
        s32 t3;
        s32 k;

        k = 2;
        v1 = (u8 *)((s32)a3 + k * 20);
        *(s8 *)(a3 + 72) = 0;
        *(s8 *)(a3 + 92) = 0;
        *(s8 *)(v1 + 92) = 0;
        *(s8 *)(v1 + 112) = 0;
        *(s8 *)(v1 + 132) = 0;
        *(s8 *)(v1 + 72) = 0;

        t3 = *(u16 *)(a3 + 942);
        *(s8 *)(a3 + 834) = 0;
        *(s8 *)(a3 + 835) = 1;
        *(s16 *)(a3 + 942) = (s16)(t3 + 8);
        *(s16 *)(a3 + 944) = 0;
        *(s16 *)(a3 + 868) = 0;
        *(s16 *)(a3 + 8) = 0;
        break;
    }
    }
}
