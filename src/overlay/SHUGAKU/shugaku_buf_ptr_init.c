/* Address: 0x80139CB0 */
/* Size: 64 bytes (16 insns) */
#include "types.h"

extern s32 shugaku_cw97_res;
extern s32 shugaku_cw97_res_img;
extern s32 shugaku_cw97_res_anim_tbl;
extern s32 shugaku_cw97_res_pal;

void shugaku_buf_ptr_init(void) {
    shugaku_cw97_res = 0x801E20DC;
    shugaku_cw97_res_img = 0x801E20F8;
    shugaku_cw97_res_anim_tbl = 0x801E2114;
    shugaku_cw97_res_pal = 0x801E0000;
}
