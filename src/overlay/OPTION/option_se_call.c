/* @SymbolStart: 0x80139F24 */
/* @SymbolSize: 136 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x78 */
/* option_se_call - OPTION overlay, Address: 0x80139F2C, Size: 136 bytes (34 insns) */
#include "types.h"

extern volatile s32 option_se_no;
extern void device_cmd_send();
extern s32 pad_data;

void option_se_call(void) {
    s32 hidden_v1;
    s32 t6;
    s32 t7;
    s32 v0;
    s32 a0;

    hidden_v1 = pad_data;

    t6 = hidden_v1 & 1;
    if (t6 != 0) {
        v0 = option_se_no;
        v0 = v0 - 1;
        option_se_no = v0;
    }
    v0 = option_se_no;
    t7 = hidden_v1 & 4;
    if (t7 != 0) {
        v0 = v0 + 1;
    }
    if (v0 < 0) {
        v0 = 32;
    }
    if (v0 >= 33) {
        v0 = 0;
    }
    option_se_no = v0;
    a0 = (v0 | 0x0500) & 0xFFFF;
    device_cmd_send(a0);
}
