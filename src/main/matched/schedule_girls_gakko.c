/* Function: schedule_girls_gakko */
/* Address: 0x80070B94 */
/* Size: 368 bytes (92 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x3c */
/* @PostProc: patch_insn_reg_field --patch 0xbc:rt:v0:t0 --patch 0xc0:rt:v1:t1 --patch 0xc8:rt:v0:t0 --patch 0xd0:rt:v1:t1 --patch 0xec:rt:t1:t3 --patch 0xf0:rt:v0:t4 --patch 0xf8:rt:t0:t2 --patch 0xfc:rs:t0:t2 --patch 0x100:rt:t1:t3 --patch 0x108:rt:v0:t4 --patch 0x11c:rt:t2:t5 --patch 0x124:rt:t2:t5 */
#include "types.h"

extern u8 screen_w[];
extern u8 schedule_buf_end;
extern u8 byouki_kind;

extern s32 girls_event_pick(u8 *, u8 *);
extern s32 date_sasoi_judge(u8 *);
extern s32 geko_judge(u8 *);
extern void sub_sub_step_up(void);

void schedule_girls_gakko(void) {
    s32 s1;
    u8 *s0;
    u8 sp5F;
    u8 sp5E;

    s1 = 0;
    s0 = screen_w;

    do {
        if (s0[1692] == 68) {
            u8 val = s0[1696];
            if (val == 51 || val == 50 || val == 71) {
                s0[1692] = 250;
            } else if ((byouki_kind & 0xF) == 3) {
                s0[1692] = 250;
            } else if (girls_event_pick(&sp5F, &sp5E) != 0) {
                u8 t0 = sp5F;
                u8 t1 = sp5E;
                s0[1692] = 69;
                s0[1693] = t0;
                s0[1694] = t1;
            } else if (date_sasoi_judge(&sp5F) == 1 && s1 == 0) {
                u8 t4 = sp5F;
                s1 = (s1 + 1) & 0xFF;
                s0[1692] = 70;
                s0[1693] = t4;
            } else if (geko_judge(&sp5F) != 0) {
                s0[1693] = sp5F;
            } else {
                s0[1692] = 250;
            }
        }
        s0 += 4;
    } while (s0 != &schedule_buf_end);

    sub_sub_step_up();
}
