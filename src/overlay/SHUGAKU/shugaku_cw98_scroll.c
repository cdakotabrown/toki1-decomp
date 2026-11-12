/* @SymbolStart: 0x80135260 */
/* @SymbolSize: 372 */
/* @PostProc: patch_stack_complete --old 32 --new 56 */
/* @PostProc: remap_stack_slots --from 28 --to 48 --from 24 --to 44 */
/* @PostProc: patch_insn_reg_field 0x48:rt:v0:v1 0x50:rs:v0:v1 0x50:rt:v0:v1 0x7c:rt:v0:v1 0x80:rt:v0:v1 0x90:rs:v0:v1 */
#include "types.h"

extern u8  voice_level;
extern s32 pad_trig;
extern u8  now_sub_sub_step;
extern s32 step_counter;
extern s16 cw98_x;
extern u8  yazirushi_cw_disp;
extern s16 with_yoshi;
extern s16 move_in;

extern void func_801343A0(void);
extern s32  check_end_k(void);
extern s32  get_CD_l_com(void);

void shugaku_cw98_scroll(void)
{
    volatile s32 selsave;
    s32 cnt;
    s16 t1v;
    s16 t3;
    s16 t3b;
    s32 cnt2;
    s32 cnt3;

    if (voice_level == 2) {
        voice_level = 1;
    }

    selsave = now_sub_sub_step;
    cnt = step_counter;
    if ((pad_trig & 0x600060) != 0) {
        t1v = cw98_x;
        if (t1v >= -191) {
            goto no_call;
        }
    }
    func_801343A0();
no_call:
    t3 = cw98_x;
    step_counter = cnt + 1;
    if (t3 >= -191) {
        cnt2 = step_counter;
        if ((cnt2 & 1) == 0) {
            if (check_end_k() != 0) {
                if (voice_level == 0) {
                    goto dec;
                }
                if (get_CD_l_com() == 9) {
                dec:
                    cw98_x = (s16)(cw98_x - 1);
                }
            }
        }
        yazirushi_cw_disp = (u8)(yazirushi_cw_disp & 0x7F);
    }

    cnt3 = step_counter;
    if (selsave != cnt3) {
        t3b = cw98_x;
        if (t3b >= -191) {
            now_sub_sub_step = (u8)selsave;
            with_yoshi = 9;
            move_in = 0;
        } else {
            with_yoshi = 9;
            move_in = 1;
        }
    }
}
