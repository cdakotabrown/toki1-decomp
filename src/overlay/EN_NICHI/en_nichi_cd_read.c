/* @SymbolStart: 0x80132308 */
/* @SymbolSize: 112 */
/* en_nichi_cd_read - EN_NICHI overlay, Address: 0x80132308, Size: 112 bytes (28 insns) */
#include "types.h"

extern void device_cmd_send();
extern void cd_read_start2();
extern s32 get_CD_State2();
extern void sub_sub_step_up();
extern u8 step_mode;

void en_nichi_cd_read(void)
{
    u8  step = step_mode;
    s32 ret;

    if (step == 0) {
        device_cmd_send(513);
        cd_read_start2(63, (s32)0x801B0000U, 31884);
        step_mode = step_mode + 1;
        return;
    }
    ret = get_CD_State2();
    if (ret & 1) {
        sub_sub_step_up();
    }
}
