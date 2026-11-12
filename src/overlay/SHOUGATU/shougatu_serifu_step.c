/* @PostProc: patch_stack_complete --old 56 --new 80 */
/* @PostProc: remap_stack_slots --from 52 --to 64 --from 53 --to 65 --from 48 --to 68 */
/* @PostProc: patch_insn_reg_field 0x58:rd:t3:t5 0x5c:rt:t4:t9 0x60:rs:t4:t9 0x60:rt:t3:t5 0x64:rd:t5:t8 0x70:rt:t5:t8 0xa0:rt:t8:t7 0xa4:rt:t6:t0 0xa8:rs:t6:t0 0xa8:rt:t6:t0 0xac:rs:t8:t7 0xac:rt:t8:t7 0xb8:rt:t8:t7 0xbc:rt:t6:t0 0xe0:rt:t0:t2 0xe4:rs:t0:t2 0xe4:rt:t0:t2 0xf0:rs:t0:t2 0xf0:rt:t1:t3 0xf4:rt:t7:t1 0xf8:rt:t4:t9 0xfc:rs:t1:t3 0xfc:rt:t7:t1 0x100:rs:t4:t9 0x100:rt:t4:t9 0x104:rt:t2:t4 0x108:rs:t4:t9 0x108:rt:t3:t5 0x10c:rt:t8:t7 0x110:rs:t3:t5 0x110:rt:t5:t8 0x114:rt:t7:t1 0x118:rs:t5:t8 0x118:rt:t2:t4 0x11c:rs:t8:t7 0x11c:rt:t8:t7 0x120:rt:t9:t6 0x124:rs:t8:t7 0x124:rt:t6:t0 0x128:rt:t3:t5 0x12c:rs:t6:t0 0x12c:rt:t0:t2 0x134:rs:t0:t2 0x134:rt:t9:t6 0x138:rs:t7:t1 0x138:rt:t7:t1 0x140:rs:t7:t1 0x140:rt:t1:t3 0x148:rs:t1:t3 0x148:rt:t4:t9 0x150:rs:t4:t9 0x154:rt:t3:t5 0x158:rt:t2:t4 0x15c:rs:t2:t4 0x15c:rt:t2:t4 0x168:rt:t8:t7 0x16c:rs:t8:t7 0x16c:rt:t8:t7 0x178:rs:t5:t8 0x178:rt:t8:t7 0x164:rt:t2:t4 0x170:rt:t5:t8 */
/* Address: 0x80133B48 */
/* Size: 408 bytes (102 instructions) */
#include "types.h"

typedef struct {
    u8 b[3];
} Blk3;

extern void sub_step_up(void);
extern void sub_sub_step_up(void);
extern void set_kanji_string(s32, s32, s32, void *, s32);
extern void msg_str_disp(s32, s32, s32, void *, s32, void *, void *, s32);
extern s32 strlen(void *);
extern void shougatu_serifu_run(void);

extern u8 p_name[];
extern u8 now_girl;
extern u8 now_sub_sub_step;
extern u8 step_mode;
extern Blk3 shougatu_serifu_step_tbl;
extern u8 shougatu_serifu_step_str0[];
extern u8 shougatu_serifu_step_str1[];
extern u8 shougatu_serifu_step_suf_str[];
extern u8 shougatu_serifu_step_bul_str[];
extern u8 ***shougatu_meet_ss_serifu;
extern u8 **shougatu_meet_ss_serifu_kind;
extern s16 shougatu_meet_now_serifu;
extern s16 shougatu_meet_now_serifu_in;

void shougatu_serifu_step(void)
{
    Blk3 buf;
    s32 saved;
    s32 r;

    buf = shougatu_serifu_step_tbl;
    if (now_girl == 0) goto tail429FC;
    if (step_mode == 0) {
        r = strlen(p_name);
        set_kanji_string((s16)(-104 - r * 4), 48, 0, p_name, 0);
        set_kanji_string(-86, 48, 0, shougatu_serifu_step_str0, 0);
        msg_str_disp(-72, 48, 0, shougatu_serifu_step_str1, 0, shougatu_serifu_step_suf_str, shougatu_serifu_step_bul_str, 0);
        shougatu_meet_now_serifu = 0;
        shougatu_meet_now_serifu_in = 0;
        **shougatu_meet_ss_serifu_kind = 7;
        (**shougatu_meet_ss_serifu)[0] = buf.b[0];
        (**shougatu_meet_ss_serifu)[1] = buf.b[1];
        (**shougatu_meet_ss_serifu)[2] = 0;
        step_mode = 1;
    }
    saved = now_sub_sub_step;
    shougatu_serifu_run();
    if (now_sub_sub_step != saved) {
        sub_step_up();
    }
    return;

tail429FC:
    sub_sub_step_up();
}
