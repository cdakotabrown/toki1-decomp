/* name_ent_prologue_serifu12_kuchi_ani - NAME_ENT overlay, Address: 0x801479BC, Size: 364 bytes (91 insns) */
/* @SymbolSize: 364 */
/* @PostProc: patch_stack_complete --old 40 --new 56 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* @PostProc: patch_insn_reg_field 0x28:rd:v1:a1 0x38:rd:v1:a1 0x54:rt:v1:a1 0x80:rd:a0:v1 0x84:rs:a0:v1 0x88:rd:v0:t6 0x88:rt:a0:v1 0x8c:rs:a0:v1 0x90:rd:v0:t6 0x98:rs:v0:t6 0xa0:rt:v1:a1 0xa4:rd:a1:a0 0xa8:rs:a1:a0 0xb4:rt:a1:a0 0xbc:rt:a1:a0 0xc0:rt:a1:a0 0x134:rt:a1:a0 */
#include "types.h"

extern s32 get_CD_State(void);
extern s32 SD_CDLevelGet(void);

extern s32 frame_counter;
extern s32 name_ent_kuchi_level_buf[];
extern s32 name_ent_kuchi_now;

extern u8  cal_cw_prio;
extern u8  cal_cw_mode;
extern u8  cal_cw_disp;
extern u8  cal_cw_sel;
extern u8  cal_cw_clut;
extern s16 cal_cw_frame;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern u32 cal_cw_img;
extern u32 cal_cw_anim_tbl;
extern u32 cal_cw_hdr;
extern u32 cal_cw_attr;

void name_ent_prologue_serifu12_kuchi_ani(void)
{
    s32 sel;
    s32 sum;
    s32 rating;

    if (get_CD_State() & 4) {
        sel = SD_CDLevelGet();
    } else {
        sel = 0;
    }

    name_ent_kuchi_level_buf[frame_counter & 3] = sel;
    sum = name_ent_kuchi_level_buf[0] + name_ent_kuchi_level_buf[1] + name_ent_kuchi_level_buf[2] + name_ent_kuchi_level_buf[3];
    rating = (sum / 4) / 10;
    name_ent_kuchi_now = sel;
    if (rating >= 4) {
        rating = 3;
    }

    if (get_CD_State() & 4) {
        cal_cw_prio = 9;
        cal_cw_mode = 64;
        cal_cw_attr = 0x41000000;
        cal_cw_disp = 164;
        cal_cw_img = 0x80180078;
        cal_cw_anim_tbl = 0x80180088;
        cal_cw_hdr = 0x80180074;
        cal_cw_frame = rating;
        cal_cw_sel = 128;
        cal_cw_clut = 0;
        cal_cw_x = -160;
        cal_cw_y = -120;
    } else {
        cal_cw_frame = 0;
    }
}
