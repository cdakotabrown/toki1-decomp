/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=2, IDO52_AS1_FORCE_LA_PULL=3, */
/* @PostProc: patch_insn_reg_field 0x90:rt:a0:v1 0x9c:rt:t8:a0 0xa0:rs:t8:a0 0xa0:rt:t8:a0 0xa8:rs:t8:a0 0xac:rt:t7:t8 0xb4:rt:t7:t8 0xe0:rt:a1:t7 0xe4:rs:a1:t7 0xe8:rs:a1:t7 0xb8:rs:a0:v1 0xb8:rt:a0:v1 0xc0:rs:a0:v1 0xc4:rt:v1:a0 0xc8:rs:v1:a0 0xc8:rt:v1:a0 0xd0:rs:a0:v1 0xd0:rt:a0:v1 0xd4:rt:a0:v1 0x100:rs:v1:a0 0x10c:rs:v1:a0 */
/* kangei_frame_check - KANGEI overlay, Address: 0x80135688, Size: 300 bytes (75 insns) */
/* explicit `@CompileFlags: -Wc,-pic0` above. */
#include "types.h"

typedef struct {
    u32 w[32];
} Table128;

extern Table128 kangei_frame_check_tbl;
extern u8  now_sub_sub_step;
extern u8  bustup_cw_mode;
extern volatile u8  now_girl;
extern s32 step_state;
extern u8  bustup2_cw_disp;
extern volatile u8  cal_cw_mode;
extern volatile u8  cal_cw_disp;

s32 kangei_frame_check(void)
{
    u32 idx;
    Table128 localTable;
    s32 counter;
    u32 remainder;
    s32 charId;
    s32 ret;

    localTable = kangei_frame_check_tbl;

    idx = now_sub_sub_step;
    ((void (*)(s32))localTable.w[idx])(128);

    if ((bustup_cw_mode & 0x10) != 0) {
        if (now_girl != 5) {
            bustup_cw_mode = 3;
        }
    }
    counter = step_state;
    remainder = (u32)counter % 300;
    charId = now_girl;
    step_state = counter + 1;
    ret = (remainder == 0);
    if (ret == 0) {
        goto out;
    }
    if ((bustup2_cw_disp & 0x80) == 0) {
        goto out;
    }
    if (charId == 10) {
        goto set_flags;
    }
    if (charId != 3) {
        goto out;
    }
set_flags:
    cal_cw_mode = 9;
    cal_cw_disp = cal_cw_disp | 0x80;
out:
    return ret;
}
