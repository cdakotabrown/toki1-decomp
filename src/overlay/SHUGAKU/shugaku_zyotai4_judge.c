/* @SymbolSize: 304 */
/* @PostProc: patch_stack_complete --old 32 --new 56 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x1C */
/* @PostProc: swap_instructions --offset1 0xA4 --offset2 0xA8 */
/* @PostProc: patch_insn_reg_field 0x14:rt:a0:v0 0x18:rs:a0:v0 0x38:rt:t7:t8 0x3c:rs:t7:t8 0x40:rt:t8:t9 0x44:rs:t8:t9 0x44:rt:t8:t9 0x4c:rs:t8:t9 0x68:rt:t9:t0 0x6c:rs:t9:t0 0x6c:rt:t9:t0 0x78:rt:t1:t2 0x7c:rs:t1:t2 0x7c:rt:t1:t2 0x88:rs:t0:t1 0x88:rt:t1:t2 0x8c:rt:t2:t3 0x98:rt:t2:t3 0xa0:rt:t2:t3 0xa4:rt:t3:v0 0xac:rt:t3:v0 0x74:rt:t9:t0 0x80:rt:t0:t1 */
#include "types.h"

extern u8  now_girl;
extern u8  now_sub_sub_step;
extern u8  date_place;
extern u8  shugaku_zyotai4_girl;
extern u8  g_name2;

extern s32  get_g_zyotai_s(s32 a0);
extern void fade_down_main(void);
extern void date_name_set(s32 a0);
extern void get_g_name(void *a0, s32 a1);
extern void sub_sub_step_up(void);

s32 shugaku_zyotai4_judge(void)
{
    s32 v0;
    s32 t0;
    s32 t3;

    v0 = get_g_zyotai_s((s32)now_girl);
    shugaku_zyotai4_girl = 255;
    if ((v0 & 0x7F) == 4 && date_place == 0) {
        v0 = (s32)now_girl;
        shugaku_zyotai4_girl = (u8)v0;
        if (v0 != 1) {
            t3 = 1;
            goto advance;
        }
        t0 = (s32)now_sub_sub_step;
        fade_down_main();
        t3 = 1;
        if (t0 != (s32)now_sub_sub_step) {
            goto advance;
        }
        return 0;
advance:
        date_place = (u8)t3;
        now_girl = 13;
        date_name_set(13);
        get_g_name(&g_name2, (s32)shugaku_zyotai4_girl);
        sub_sub_step_up();
        goto done;
    }
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    if (date_place != 0) {
        sub_sub_step_up();
        sub_sub_step_up();
        sub_sub_step_up();
        sub_sub_step_up();
    }
done:
    ;
}
