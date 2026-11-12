/* @CompileEnv: IDO52_UGEN_TREG_ORDER=t6,t8,t9,t0,t1,t2,t3,t4,t5 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --from 31 --to 43 --from 30 --to 41 */
/* @PostProc: patch_insn_reg_field 0xfc:rd:t6:t7 0x100:rs:t6:t7 */
/* Address: 0x8013E26C */
/* Size: 392 bytes (98 instructions) */
#include "types.h"

extern s32 get_g_zyotai_s(s32);
extern s32 get_g_zyotai_h(s32);
extern void sub_sub_step_up(void);

extern s16 girl0_syoushin;
extern s16 girl7_syoushin;
extern s32 girl7_flag;
extern s16 girl9_syoushin;
extern s32 girl9_flag;
extern u8 ev_flag06;
extern u8 ev_flag34;
extern u8 ev_flag42;
extern u8 shougatu_menu_ev_girl;

void shougatu_girl_judge2(void)
{
    u8 probe;
    u8 best;
    s32 r;

    probe = (u8)get_g_zyotai_s(7);
    r = get_g_zyotai_h(7);
    best = 255;
    if (((girl7_flag << 30) < 0) && ev_flag34 == 0 && probe == 128 &&
        r == 0) {
        best = 7;
    }
    probe = (u8)get_g_zyotai_s(0);
    r = get_g_zyotai_h(0);
    if (ev_flag06 != 0) goto stage9;
    if (probe != 128) goto stage9;
    if (r != 0) goto stage9;
    if (best == 255) goto take0;
    if (best != 7) goto stage9;
    if (girl0_syoushin >= girl7_syoushin) goto stage9;
take0:
    best = 0;

stage9:
    probe = (u8)get_g_zyotai_s(9);
    r = get_g_zyotai_h(9);
    if ((girl9_flag << 30) >= 0) goto done;
    if (ev_flag42 != 0) goto done;
    if (probe != 128) goto done;
    if (r != 0) goto done;
    if (best == 255) goto take9;
    if (best != 7) goto chk0;
    if (girl9_syoushin < girl7_syoushin) goto take9;
chk0:
    if (best) goto done;
    if (girl9_syoushin >= girl0_syoushin) goto done;
take9:
    best = 9;

done:
    shougatu_menu_ev_girl = best;
    sub_sub_step_up();
}
