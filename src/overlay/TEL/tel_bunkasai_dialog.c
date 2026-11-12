/* @SymbolStart: 0x8013CAC0 */
/* @SymbolSize: 304 */
/* @RodataBase: 0x80140388 */
/* tel_bunkasai_dialog - TEL overlay, Address: 0x8013CAC8, Size: 296 bytes (74 insns) */
#include "types.h"

extern u8 step_mode;
extern s32 tel_bunkasai_dialog_str0;
extern s32 tel_bunkasai_dialog_str1;
extern s32 p_sei;
extern s32 tel_bunkasai_dialog_str2;

extern void xa_wait(void);
extern void k_sub_reset(void);
extern void gnsx(s32);
extern void sndisp(s32, s32, s32);
extern void xa_set_m(s32, s32, s32, s32);
extern void k_sub_disp_start(s32);
extern void xa_wait_m(void);
extern void goto_step(s32);

void tel_bunkasai_dialog(void)
{
    u8 hidden_t6;

    hidden_t6 = step_mode;
    switch ((u32)hidden_t6) {
        case 0:
            xa_wait();
            break;
        case 1:
            k_sub_reset();
            gnsx((s32)&tel_bunkasai_dialog_str0);
            sndisp((s32)&tel_bunkasai_dialog_str1, 0, 13);
            xa_set_m(0x0D000001, 0x02000010, -1, 14);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 2:
            xa_wait_m();
            break;
        case 3:
            k_sub_reset();
            gnsx((s32)&p_sei);
            sndisp((s32)&tel_bunkasai_dialog_str2, 0, 31);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 4:
            xa_wait();
            break;
        default:
            goto_step(52);
            break;
    }
}
