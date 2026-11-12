/* @SymbolStart: 0x8013A7BC */
/* @RodataBase: 0x8013FFF8 */
/* @SymbolSize: 420 */
/* tel_tome_dialog - TEL overlay, Address: 0x8013A7C4, Size: 420 bytes (105 insns) */
#include "types.h"

extern u8 step_mode;
extern u8 p_sei;
extern u8 tel_tome_dialog_str0;
extern u8 tel_tome_dialog_str1;
extern u8 tel_tome_dialog_str2;
extern u8 tel_tome_dialog_str3;
extern u8 tel_tome_dialog_str4;

extern void xa_wait(void);
extern void device_cmd_send(s32);
extern void k_sub_reset(void);
extern void gnsx(s32);
extern void sndisp(s32, s32, s32);
extern void xa_set_m(s32, s32, s32, s32);
extern void k_sub_disp_start(s32);
extern void xa_wait_m(void);
extern void sub_sub_step_up(void);

void tel_tome_dialog(void)
{
    u8 hidden_t6;

    hidden_t6 = step_mode;
    switch (hidden_t6) {
        case 0:
            xa_wait();
            return;
        case 1:
            device_cmd_send(1290);
            k_sub_reset();
            gnsx((s32)&tel_tome_dialog_str0);
            sndisp((s32)&tel_tome_dialog_str1, 0, 13);
            xa_set_m(0x1D000095, 0x1C00009B, -1, 14);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            return;
        case 2:
            xa_wait_m();
            return;
        case 3:
            k_sub_reset();
            gnsx((s32)&tel_tome_dialog_str2);
            sndisp((s32)&tel_tome_dialog_str3, 0, 13);
            xa_set_m(0x0C000022, 0x05000039, -1, 14);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            return;
        case 4:
            xa_wait_m();
            return;
        case 5:
            k_sub_reset();
            gnsx((s32)&p_sei);
            sndisp((s32)&tel_tome_dialog_str4, 0, 15);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            return;
        case 6:
            xa_wait();
            return;
    }
    sub_sub_step_up();
}
