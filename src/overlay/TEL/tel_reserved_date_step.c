/* @RodataBase: 0x8013FE88 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=21:v1 */
/* tel_reserved_date_step - TEL overlay, Address: 0x80139F1C, Size: 572 bytes (143 insns) */
/* @SymbolSize: 572 */
/* @PostProc: remap_stack_slots --map '52:48' */
#include "types.h"

extern volatile s32 reserved_date;
extern u8 step_mode;
extern s32 date_name_set_tbl0[];
extern s32 tel_yakusoku_yaburi1_serifu_tbl[];
extern u8 tel_yakusoku_yaburi1_xa_tbl[];
extern u8 tel_yakusoku_yaburi1_xa_tbl2[];
extern s32 tel_yakusoku_yaburi2_serifu_tbl[];
extern u8 tel_yakusoku_yaburi2_xa_tbl[];
extern u8 tel_yakusoku_yaburi2_xa_tbl2[];
extern s32 tel_yakusoku_yaburi3_serifu_tbl[];
extern u8 tel_yakusoku_yaburi3_xa_tbl[];
extern u8 tel_yakusoku_yaburi3_xa_tbl2[];

extern s32 get_g_zyotai_s(s32);
extern void xa_wait(void);
extern void k_sub_reset(void);
extern void gnsx(s32);
extern void sndisp(s32, s32, s32);
extern void xa_set_m(s32, s32, s32, s32);
extern void k_sub_disp_start(s32);
extern void xa_wait_m(void);
extern void sub_sub_step_up(void);

void tel_reserved_date_step(void)
{
    s32 charIdx;
    s32 result;

    charIdx = reserved_date;
    charIdx = (s32)(((u32)(charIdx << 3)) >> 28);
    result = get_g_zyotai_s(charIdx);

    switch (step_mode) {
    case 0:
        xa_wait();
        return;
    case 1:
        if (result & 0x80) {
            step_mode = 10;
        } else {
            step_mode = 50;
        }
        return;
    case 10:
        k_sub_reset();
        gnsx(date_name_set_tbl0[charIdx]);
        sndisp(tel_yakusoku_yaburi1_serifu_tbl[charIdx], 0, charIdx);
        xa_set_m((s32)tel_yakusoku_yaburi1_xa_tbl, (s32)tel_yakusoku_yaburi1_xa_tbl2, charIdx, charIdx);
        k_sub_disp_start(0);
        step_mode = step_mode + 1;
        return;
    case 11:
        xa_wait_m();
        return;
    case 30:
        k_sub_reset();
        gnsx(date_name_set_tbl0[charIdx]);
        sndisp(tel_yakusoku_yaburi2_serifu_tbl[charIdx], 0, charIdx);
        xa_set_m((s32)tel_yakusoku_yaburi2_xa_tbl, (s32)tel_yakusoku_yaburi2_xa_tbl2, charIdx, charIdx);
        k_sub_disp_start(0);
        step_mode = step_mode + 1;
        return;
    case 31:
        xa_wait_m();
        return;
    case 50:
        k_sub_reset();
        gnsx(date_name_set_tbl0[charIdx]);
        sndisp(tel_yakusoku_yaburi3_serifu_tbl[charIdx], 0, charIdx);
        xa_set_m((s32)tel_yakusoku_yaburi3_xa_tbl, (s32)tel_yakusoku_yaburi3_xa_tbl2, charIdx, charIdx);
        k_sub_disp_start(0);
        step_mode = step_mode + 1;
        return;
    case 51:
        xa_wait_m();
        return;
    case 70:
        break;
    }
    sub_sub_step_up();
}
