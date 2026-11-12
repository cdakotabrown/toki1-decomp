/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=6,8 IDO52_COPYPROP_SITE=lod:10:deny IDO52_AS1_TIE_TAKE=142 */
/* @PostProc: swap_instructions --offset1 0x160 --offset2 0x164 */
/* @PostProc: swap_instructions --offset1 0x210 --offset2 0x218 */
/* @PostProc: remap_stack_slots --map 60:84 */
/* @PostProc: remap_stack_slots --map 56:76 */
/* @PostProc: patch_insn_reg_field 0x88:rt:a2:a1 0x90:rs:a2:a1 0x98:rs:a2:a1 0xa0:rs:a2:a1 0xb4:rs:a2:a1 0xbc:rs:a2:a1 0xc4:rs:a2:a1 0xd8:rs:a2:a1 0xe0:rs:a2:a1 0xe8:rs:a2:a1 0x104:rs:a2:a1 0x12c:rd:a3:a2 0x130:rs:a3:a2 0x130:rt:a1:v1 0x138:rs:a1:v1 0x144:rt:v1:t4 0x14c:rs:v1:t4 0x154:rd:a3:a2 0x158:rs:a3:a2 0x158:rt:a1:v1 0x15c:rt:t4:t5 0x160:rs:t4:t5 0x160:rt:t4:t5 0x164:rt:a2:a1 0x168:rs:a1:v1 0x16c:rt:t4:t5 0x170:rt:v1:v0 0x174:rs:a2:a1 0x178:rs:v1:v0 0x17c:rs:a2:a1 0x17c:rt:t5:t6 0x180:rs:a1:v1 0x184:rt:t5:t6 0x188:rt:v1:v0 0x190:rs:v1:v0 0x194:rs:v1:v0 0x194:rt:t1:t2 0x198:rt:v0:a1 0x1a0:rs:v0:a1 0x1ac:rt:t6:t7 0x1b0:rt:v0:a1 0x1b4:rs:t6:t7 0x1b4:rt:t7:t8 0x1b8:rt:t7:t8 0x1bc:rt:t8:t9 0x1c0:rs:v0:a1 0x1c0:rt:t0:t1 0x1c4:rs:t8:t9 0x1c4:rt:t9:t0 0x1c8:rt:t9:t0 0x1cc:rt:t0:t1 0x1e0:rt:t1:t2 0x1e4:rs:a1:v1 0x1e4:rt:t2:t3 0x1f0:rs:a3:a2 0x1f0:rt:t2:t3 0x1f8:rs:a1:v1 0x218:rt:a3:a2 0x224:rt:a3:a2 0x254:rs:a3:a2 0x258:rd:v1:t4 0x25c:rs:v1:t4 0x25c:rt:t3:t5 0x264:rt:t3:t5 0x270:rs:a1:v1 0x27c:rt:t4:t8 0x280:rt:v1:t6 0x284:rt:t4:t8 0x288:rs:v1:t6 0x288:rt:t5:t7 0x290:rt:t5:t7 0x294:rs:a1:v1 0x294:rt:t6:t9 0x2a0:rs:a3:a2 0x2a0:rt:t6:t9 0x2a8:rt:t7:t2 0x2ac:rt:v1:t0 0x2b0:rt:t7:t2 0x2b4:rs:v1:t0 0x2b4:rt:t8:t1 0x2bc:rt:t8:t1 0x2c0:rs:a1:v1 0x2c0:rt:t9:t3 0x2cc:rs:a3:a2 0x2cc:rt:t9:t3 0x2d8:rs:a1:v1 0x2e0:rs:v1:v0 0x31c:rd:t0:t4 0x320:rs:t0:t4 0x320:rt:t1:t5 0x324:rt:t1:t5 0x32c:rd:t2:t6 0x330:rt:t2:t6 */
/* geko_anim -- GEKO overlay, Address: 0x8013E8A8, Size: 856 bytes (214 insns) */
#include "types.h"

extern s32 rand(void);

typedef struct {
    s32 f0;
    s16 f4;
    s16 f6;
    s16 f8;
    s16 f10;
    s16 f12;
    s8  f14;
    s8  f15;
    s16 f16;
    s16 f18;
    u8  f20;
    u8  f21;
    u8  f22;
    s16 f24;
    s16 f26;
    s16 f28;
    s16 f30;
    s32 f32;
} Ent;

extern Ent Sprites[];
extern s16 geko_ptc_phase[];
extern s16 geko_ptc_sub[];

void geko_anim(void)
{
    s16 i;
    Ent *e;
    s32 y;
    s32 a;
    s32 b;
    s32 b2;
    s32 b3;
    s32 r;

    for (i = 4; i < 14; i++)
    {
        e = &Sprites[i];
        e->f6 = e->f6 + 10;

        if ((rand() & 3) == 0)
        {
            y = e->f6;
            if ((-82 <= y && y < -57 && e->f4 < -87) ||
                (-57 <= y && y < -26 && e->f4 < -119) ||
                (-11 <= y && y < 3 && -141 <= e->f4 && e->f4 < -111) ||
                (5 <= y && -127 <= e->f4 && e->f4 < -92))
            {
                a = geko_ptc_phase[i - 4];
                if (a == 0)
                {
                    b = geko_ptc_sub[i - 4];
                    if (b == 0)
                    {
                        goto la14;
                    }
                }
            }
        }

        a = geko_ptc_phase[i - 4];
        y = e->f6;
        if (a != 0 || (b = geko_ptc_sub[i - 4]) != 0)
        {
        la14:
            e->f6 = y - 10;
            if (a == 0)
            {
                b2 = geko_ptc_sub[i - 4];
                if (b2 == 0)
                {
                    if (e->f6 < -68)
                    {
                        e->f15 = (s8)((u8)e->f15 + 8);
                    }
                    e->f4 = e->f4 - 12;
                    e->f6 = e->f6 + 16;
                    e->f14 = 8;
                    e->f8 = 24;
                    e->f10 = 8;
                    geko_ptc_sub[i - 4] = b2 + 1;
                }
                else
                {
                    geko_ptc_sub[i - 4] = 0;
                    geko_ptc_phase[i - 4] = a + 1;
                }
            }
            else if (a == 4)
            {
                e->f14 = 0;
                e->f15 = 0;
                e->f8 = 8;
                e->f10 = 24;
                r = rand() % 84;
                e->f6 = -120;
                geko_ptc_sub[i - 4] = 0;
                geko_ptc_phase[i - 4] = 0;
                e->f4 = r - 159;
            }
            else
            {
                b3 = geko_ptc_sub[i - 4];
                if (a == 5)
                {
                    if (b3 == 0)
                    {
                        r = (u8)e->f14;
                        geko_ptc_sub[i - 4] = b3 + 1;
                        e->f14 = (s8)(r - 24);
                    }
                    else
                    {
                        geko_ptc_sub[i - 4] = 0;
                        geko_ptc_phase[i - 4] = a + 1;
                    }
                }
                else
                {
                    if (b3 == 0)
                    {
                        r = (u8)e->f14;
                        geko_ptc_sub[i - 4] = b3 + 1;
                        e->f14 = (s8)(r + 24);
                    }
                    else
                    {
                        geko_ptc_sub[i - 4] = 0;
                        geko_ptc_phase[i - 4] = a + 1;
                    }
                }
            }
        }
        else if (y >= 25 && a == 0 && b == 0)
        {
            r = rand() % 84;
            e->f6 = -120;
            e->f4 = r - 159;
        }
    }
}
