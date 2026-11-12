/* @SymbolSize: 112 */
/* @PostProc: patch_insn_reg_field 0x50:rd:at:v1 0x54:rs:at:v1 */
#include "types.h"

extern s32 device_cmd_send();
extern s32 device_ready_check();
extern s32 change_trans_pointer();
extern s32 SD_CmdProc();
extern s32 VSync();

void tt_load_wait2(void)
{
    s32 s0, s1;

    SD_CmdProc();
    change_trans_pointer((void *)0x8019A000);
    device_cmd_send(18);
    s0 = 0;
    s1 = 0x10000;
L:
    SD_CmdProc();
    VSync(0);
    if (s0 < s1) {
        s0 += 1;
        if (device_ready_check(1) == 0) {
            goto L;
        }
    }
}
