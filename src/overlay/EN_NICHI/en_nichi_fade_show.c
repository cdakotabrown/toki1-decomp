/* @SymbolSize: 516 */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=0 */
/* en_nichi_fade_show - EN_NICHI overlay, Address: 0x8013687C, Size: 516 bytes (129 insns) */
/* @PostProc: patch_stack_complete --old 48 --new 56 */
/* @PostProc: remap_stack_slots --from 36 --to 48 */
/* @PostProc: patch_insn_reg_field 0x44:rt:a2:a1 0x50:rs:a2:a1 0x5c:rs:a2:a1 0x68:rs:a2:a1 0x8c:rs:a2:a1 0xac:rs:a2:a1 0xcc:rs:a2:a1 0xec:rs:a2:a1 0x10c:rs:a2:a1 0x12c:rs:a2:a1 0x14c:rs:a2:a1 0x16c:rs:a2:a1 0x1a0:rt:v1:v0 0x1a4:rs:v1:v0 0x1a4:rt:v1:v0 0x1ac:rs:v1:v0 0x1b8:rs:v1:v0 */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 prim_alloc();
extern void SetPolyF4();
extern void SetSemiTrans();
extern void AddPrim();
extern void dtd_on_tpage();
extern void sub_sub_step_up();

extern s32 activeBuff;
extern s32 en_nichi_fade_lv;
extern s16 screen_w[];
extern u8  ot_buff0[];
extern CLASS_W class_w[];

void en_nichi_fade_show(void)
{
    s32 ci;
    s32 v0;
    u8 *a1;
    s16 *p;

    a1 = (u8 *)prim_alloc(24, activeBuff);
    SetPolyF4((s32)a1);
    SetSemiTrans((s32)a1, 1);

    a1[4] = (u8)en_nichi_fade_lv;
    a1[5] = (u8)en_nichi_fade_lv;
    a1[6] = (u8)en_nichi_fade_lv;

    ci = activeBuff;
    p = &screen_w[ci];
    *(s16 *)(a1 + 8) = p[10] - 88;

    ci = activeBuff;
    p = &screen_w[ci];
    *(s16 *)(a1 + 10) = p[12] - 64;

    ci = activeBuff;
    p = &screen_w[ci];
    *(s16 *)(a1 + 12) = p[10] + 88;

    ci = activeBuff;
    p = &screen_w[ci];
    *(s16 *)(a1 + 14) = p[12] - 64;

    ci = activeBuff;
    p = &screen_w[ci];
    *(s16 *)(a1 + 16) = p[10] - 88;

    ci = activeBuff;
    p = &screen_w[ci];
    *(s16 *)(a1 + 18) = p[12] + 48;

    ci = activeBuff;
    p = &screen_w[ci];
    *(s16 *)(a1 + 20) = p[10] + 88;

    ci = activeBuff;
    p = &screen_w[ci];
    *(s16 *)(a1 + 22) = p[12] + 48;

    ci = activeBuff;
    AddPrim((s32)(&ot_buff0[ci * 1024 + 8]));

    dtd_on_tpage(0, 0, 2, 2, 0);

    v0 = en_nichi_fade_lv;
    if (v0 == 0) {
        class_w[155].disp_flag = 128;
    }
    v0 = v0 + 2;
    class_w[155].bright = (u8)v0;
    if (v0 >= 128) {
        class_w[155].bright = 128;
    }
    en_nichi_fade_lv = v0;
    if (v0 == 160) {
        class_w[155].attribute = 0x01000000U;
        class_w[155].clut = 17;
        sub_sub_step_up();
    }
}
