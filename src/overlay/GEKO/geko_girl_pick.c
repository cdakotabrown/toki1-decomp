/* @CompileEnv: IDO52_COPYPROP_VAR=-12 IDO52_COPYPROP_SITE=lod:1:deny,lod:6:deny IDO52_FORCE_WEB_COLOR=16:s2,59:s3,62:s4,65:s5 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_UGEN_DEST_TEMP=24 IDO52_UGEN_TEMP_REG=2:t6,4:t9,5:t0,6:t1,7:t2,8:t3,9:t4,10:t5,11:t7,12:t8,13:t9 */
/* geko_girl_pick -- GEKO overlay, Address: 0x80134A40, Size: 432 bytes (108 insns) */
#include "types.h"

extern s32  get_g_zyotai_s(s32 a0);
extern s32  get_h_yuukou(s32 a0);
extern s32  get_h_tokimeki(s32 a0);
extern void date_name_set(s32 a0);
extern s32  goto_sub_sub_step(s32 a0);
extern void geko_girl_ptr_set(void);
extern void geko_ptr_table_init1(void);
extern void sub_sub_step_up(void);

extern s32 pick_girl;

extern u8 day_flag;
extern u8 now_girl;

typedef struct {
    s32 flags;
    u8  pad[52];
} CharEntry;

extern u8 girl0_flag[];

s32 geko_girl_pick(void)
{
    s32 idx;
    s32 next;
    s32 cand;
    s32 v0;
    s32 v1;
    s32 c4;
    u32 best;
    u8  bestScore;
    u8  score;
    u8  a0;
    u8  sel;
    u8  c1;
    u8  c2;
    u8  c3;

    best = 255;
    bestScore = 4;
    idx = 0;

loop:
    if ((*(s32 *)&girl0_flag[(idx * 7) << 3] << 30) >= 0) {
        next = idx + 1;
        goto advance;
    }

    if ((day_flag & 0x80) != 0) {
        next = idx + 1;
        if (next == pick_girl) {
            goto advance;
        }
    }

    v0 = get_g_zyotai_s(idx);
    next = idx + 1;
    v1 = v0 & 0x7F;
    a0 = v1;
    score = v1;

    if ((u32)a0 < (u32)bestScore) {
        goto accept;
    }
    if ((u32)a0 != (u32)bestScore) {
        goto advance;
    }
    if ((u32)a0 != (u32)bestScore) {
        goto advance;
    }
    cand = best;

    c1 = get_h_yuukou(idx);
    c2 = get_h_tokimeki(idx);
    c3 = get_h_tokimeki(cand);
    if (!((u32)(get_h_yuukou(cand) + c3) < (u32)(c2 + c1))) {
        goto advance;
    }

accept:
    best = idx & 0xFF;
    bestScore = score;

advance:
    idx = (s32)(s16)next;
    if (idx < 11) {
        goto loop;
    }

    v0 = get_g_zyotai_s(best);
    pick_girl = 0;

    if ((u32)(v0 & 0x7F) >= 2U) {
        goto bail;
    }
    if (best == 255) {
        goto bail;
    }
    sel = best & 0xFF;
    if ((day_flag & 8) == 0) {
        goto pick;
    }

bail:
    goto_sub_sub_step(23);
    return 0;

pick:
    now_girl = sel;
    date_name_set(sel);
    geko_ptr_table_init1();
    geko_girl_ptr_set();
    sub_sub_step_up();
}
