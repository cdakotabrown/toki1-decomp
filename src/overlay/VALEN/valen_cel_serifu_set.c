/* @SymbolStart: 0x80133F20 */
/* @SymbolSize: 96 */
/* valen_cel_serifu_set - VALEN overlay, Address: 0x80133F58, Size: 40 bytes (10 insns) */
#include "types.h"

extern s32 valen_ss_s;
extern s32 valen_ss_s_kind;
extern s32 valen_sss_so;
extern s16 valen_now_s;
extern s16 valen_now_s_in;
extern s32 valen_msg_res_tbl[][13];
extern void sub_sub_step_up(void);

void valen_cel_serifu_set(void)
{
    valen_ss_s = valen_msg_res_tbl[0][4];
    valen_ss_s_kind = valen_msg_res_tbl[1][4];
    valen_sss_so = valen_msg_res_tbl[2][4];
    valen_now_s = 12;
    valen_now_s_in = 0;
    sub_sub_step_up();
}
