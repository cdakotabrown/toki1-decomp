/* @PostProc: patch_insn_reg_field 0x20:rt:t6:t7 0x24:rs:t6:t7 0x2c:rt:t7:t8 0x30:rs:t7:t8 0x30:rt:t7:t8 0x38:rs:t7:t8 */
#include "types.h"

extern s32 get_g_zyotai_s(s32 a0);
extern void bustup_cd_read(s32);
extern void sub_sub_step_up(void);

extern s32 serifu_no;

void kangei_girl0_bustup_read(void)
{
    s32 result;
    s32 counter;

    result = get_g_zyotai_s(0);
    if ((u32)(result & 0x7F) >= 2) {
        goto fallback;
    }
    counter = serifu_no;
    if (counter >= 2) {
        goto fallback;
    }
    bustup_cd_read(22388);
    goto done;

fallback:
    bustup_cd_read(21983);
done:
    sub_sub_step_up();
}
