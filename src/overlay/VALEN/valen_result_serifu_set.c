/* @SymbolStart: 0x80133ABC */
/* @SymbolSize: 216 */
/* @PostProc: patch_insn_reg_field 0x28:rt:t7:t8 0x30:rs:t7:t8 0x30:rt:t8:t9 0x34:rt:t8:t9 0x40:rt:t3:t4 0x4c:rt:t4:t5 0x64:rt:t9:t0 0x70:rt:t9:t0 0x7c:rt:t0:t1 0x88:rt:t0:t1 0x90:rt:t2:t3 0x94:rt:t1:t2 0xa0:rt:t1:t2 0xa8:rt:t2:t3 0xac:rs:t3:t4 0xac:rt:t3:t4 0xb4:rs:t4:t5 0xb4:rt:t4:t5 0xb8:rt:t3:t4 0xc0:rt:t5:t6 0xc4:rs:t5:t6 0xc4:rt:t5:t6 0xc8:rt:t4:t5 0xd4:rt:t5:t6 */
/* valen_result_serifu_set - VALEN overlay, Address: 0x80133ABC, Size: 216 bytes (54 insns) */
#include "types.h"

extern u8 valen_girl9_flag;
extern u8 girl9_flag;
extern s16 valen_girl_list_cnt;
extern s16 valen_now_s;
extern s32 valen_ss_s;
extern s32 valen_ss_s_kind;
extern s32 valen_sss_so;
extern s32 valen_msg_res_tbl[][13];
extern void sub_sub_step_up(void);

void valen_result_serifu_set(void)
{
    u8 flag;
    s16 cnt;

    flag = valen_girl9_flag;
    if (flag != 0) {
        *(u8 *)&girl9_flag |= 0x02;
    }
    cnt = valen_girl_list_cnt;
    if (cnt >= 7) {
        valen_now_s = 0;
    } else if (cnt >= 5) {
        valen_now_s = 1;
    } else if (cnt >= 3) {
        valen_now_s = 2;
    } else if (cnt > 0) {
        valen_now_s = 3;
    } else {
        valen_now_s = 4;
    }
    valen_ss_s = valen_msg_res_tbl[0][1];
    valen_ss_s_kind = valen_msg_res_tbl[1][1];
    valen_sss_so = valen_msg_res_tbl[2][1];
    sub_sub_step_up();
}
