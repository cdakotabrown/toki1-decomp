/* Size: 80 bytes */
/* @SymbolSize: 80 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t9:t0 0x54:rt:t0:t1 0x5c:rt:t0:t1 */
#include "types.h"

extern s16 shugaku_girl2_res_aux_src;
extern s32 shugaku_girl2_res;
extern s32 shugaku_girl2_res_img;
extern s32 shugaku_girl2_res_anim_tbl;
extern s16 shugaku_girl2_res_aux;
extern s32 shugaku_girl2_res_pal;

void shugaku_desc_init(void) {
    shugaku_girl2_res = 0x801C25B8;
    shugaku_girl2_res_img = 0x801C25C8;
    shugaku_girl2_res_anim_tbl = 0x801C2674;
    shugaku_girl2_res_aux = shugaku_girl2_res_aux_src;
    shugaku_girl2_res_pal = 0x801B0000;
}
