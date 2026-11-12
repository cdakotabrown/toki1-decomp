/* @PostProc: patch_insn_reg_field 0x74:rt:t2:t3 0x7c:rt:t3:t4 0x80:rt:t2:t3 0x84:rs:t3:t4 0x84:rt:t3:t5 0x8c:rt:t3:t5 0x94:rt:t4:t6 0x98:rt:t4:t6 0x9c:rt:t5:t7 0xa0:rs:t5:t7 0xa0:rt:t5:t7 0xac:rt:t5:t7 */
/* Address: 0x80134890 */
/* Size: 160 bytes (40 instructions) */
#include "types.h"

extern s32 shougatu_omikuji_res;
extern s32 shougatu_omikuji_res_hdr1;
extern s32 shougatu_omikuji_res_img0;
extern s32 shougatu_omikuji_res_img1;
extern s32 shougatu_omikuji_res_anim_tbl0;
extern volatile s32 shougatu_omikuji_res_anim_tbl1;
extern volatile s16 shougatu_omikuji_res_aux0;
extern volatile s16 shougatu_omikuji_res_aux1;
extern s32 shougatu_omikuji_res_pal0;
extern s32 shougatu_omikuji_res_pal1;
extern volatile s16 shougatu_omikuji_res_aux0_src;
extern volatile s16 shougatu_omikuji_res_aux1_src;

void shougatu_bank_init(void)
{
    shougatu_omikuji_res = 0x801DABE0;
    shougatu_omikuji_res_hdr1 = 0x801DB684;
    shougatu_omikuji_res_img0 = 0x801DABE8;
    shougatu_omikuji_res_img1 = 0x801DB6A0;
    shougatu_omikuji_res_anim_tbl0 = 0x801DAC48;
    shougatu_omikuji_res_anim_tbl1 = 0x801DB6CC;
    shougatu_omikuji_res_aux0 = shougatu_omikuji_res_aux0_src;
    shougatu_omikuji_res_aux1 = shougatu_omikuji_res_aux1_src;
    shougatu_omikuji_res_pal0 = 0x801C0000;
    shougatu_omikuji_res_pal1 = 0x801C2000;
}
