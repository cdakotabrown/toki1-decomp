/* @SymbolStart: 0x80139FAC */
/* @SymbolSize: 132 */
/* option_seq_num_step - OPTION overlay, Address: 0x80139FB4, 124 bytes (31 insns) */
#include "types.h"

extern u8 option_seq_no;
extern void device_cmd_send(s32);
extern s32 pad_trig;

void option_seq_num_step(void)
{
    s32 v1;
    s32 sel;
    s32 val;

    v1 = pad_trig;
    sel = v1 & 0x60;
    if (sel != 32) {
        if (sel == 64) {
            val = option_seq_no;
            option_seq_no = (u8)(val - 1);
        }
    } else {
        val = option_seq_no;
        option_seq_no = (u8)(val + 1);
    }

    if ((u32)option_seq_no >= 17U) {
        val = option_seq_no;
        if (val == 0 && sel == 0 && sel == 0) {

        }
        val |= 0x1200;
        val &= 0xffff;
        device_cmd_send(val);
    }
}
