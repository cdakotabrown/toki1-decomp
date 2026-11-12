/* Address: 0x80134A54 */
/* Size: 480 bytes (120 instructions) */
#include "types.h"

extern void shougatu_bank_init(void);
extern s32 block_checksum_check(s32, void *);
extern void sub_sub_step_up(void);

extern u32 *shougatu_omikuji_res;
extern u32 *shougatu_omikuji_res_hdr1;
extern u32 *shougatu_omikuji_res_anim_tbl0;
extern u32 *shougatu_omikuji_res_anim_tbl1;
extern u8 now_sub_sub_step;

s32 shougatu_data_check(void)
{
    shougatu_bank_init();
    if (shougatu_omikuji_res_hdr1[0] != 0x801CC000 || shougatu_omikuji_res_hdr1[1] != 0x801CE000 ||
        shougatu_omikuji_res_hdr1[2] != 0x801D0000 || shougatu_omikuji_res_hdr1[3] != 0x801D2000 ||
        shougatu_omikuji_res_hdr1[4] != 0x801D4000 || shougatu_omikuji_res_hdr1[5] != 0x801D6000 ||
        shougatu_omikuji_res_hdr1[6] != 0x801D8000 ||
        shougatu_omikuji_res[0] != 0x801C4000 || shougatu_omikuji_res[1] != 0x801C8000 ||
        shougatu_omikuji_res_anim_tbl1[0] != 0x801DB634 || shougatu_omikuji_res_anim_tbl1[1] != 0x801DB644 ||
        shougatu_omikuji_res_anim_tbl1[2] != 0x801DB654 ||
        shougatu_omikuji_res_anim_tbl0[0] != 0x801DAB84 || shougatu_omikuji_res_anim_tbl0[1] != 0x801DAB8C ||
        shougatu_omikuji_res_anim_tbl0[2] != 0x801DABA0 || shougatu_omikuji_res_anim_tbl0[3] != 0x801DABB4 ||
        shougatu_omikuji_res_anim_tbl0[4] != 0x801DABC0 || shougatu_omikuji_res_anim_tbl0[5] != 0x801DABCC) {
        now_sub_sub_step -= 2;
        return 0;
    }
    if (block_checksum_check(55, (void *)0x801C0000) == 0) {
        now_sub_sub_step -= 2;
        return 0;
    }
    sub_sub_step_up();
}
