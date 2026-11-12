/* @CompileFlags: -Wc,-pic1 */
/* name_ent_kuchi_anif - NAME_ENT overlay, Address: 0x80149A3C, Size: 484 bytes (121 insns) */
/* @SymbolSize: 484 */
/* @PostProc: patch_stack_complete --old 56 --new 64 */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0xd8 --offset2 0x10c */
/* @PostProc: swap_instructions --offset1 0xfc --offset2 0x10c */
/* @PostProc: swap_instructions --offset1 0x100 --offset2 0x10c */
/* @PostProc: swap_instructions --offset1 0x104 --offset2 0x10c */
/* @PostProc: swap_instructions --offset1 0x108 --offset2 0x10c */
/* @PostProc: patch_insn_reg_field 0x28:rd:v1:a0 0x38:rd:v1:a0 0x54:rt:v1:a0 0x80:rd:a0:v1 0x84:rs:a0:v1 0x88:rd:v0:t6 0x88:rt:a0:v1 0x8c:rs:a0:v1 0x90:rd:v0:t6 0x98:rs:v0:t6 0xa0:rt:v1:a0 0xd8:rt:a0:v1 0xdc:rt:v0:a3 0xe0:rs:v0:a3 0xe0:rt:v0:a3 0xfc:rt:v1:v0 0x104:rt:v1:v0 0x114:rs:a0:v1 0x114:rt:a0:v1 0x11c:rt:a2:a0 0x120:rt:a0:v1 0x124:rs:a2:a0 0x124:rt:a2:a0 0x12c:rt:a3:a2 0x130:rt:a2:a0 0x134:rs:a3:a2 0x134:rt:a3:a2 0x13c:rt:a3:a2 0x15c:rt:v0:a3 0x188:rt:v1:v0 0x19c:rt:a0:v1 0x1a4:rt:a2:a0 0x1ac:rt:a3:a2 0x1cc:rt:v0:a3 */
#include "types.h"

extern s32 get_CD_State(void);
extern s32 SD_CDLevelGet(void);
extern void class_clear_n(s32 a0, s32 a1);

extern s32 frame_counter;
extern volatile s32 name_ent_kuchi_level_buf[];
extern volatile s32 D_8014D194_a;
extern volatile s32 name_ent_kuchi_level_buf_1;
extern volatile s32 name_ent_kuchi_level_buf_2;
extern volatile s32 name_ent_kuchi_level_buf_3;
extern volatile s32 name_ent_kuchi_now;

extern volatile u8  cw97_prio;
extern volatile u8  cw97_mode;
extern volatile u8  cw97_disp;
extern volatile u8  cw97_sel;
extern volatile s16 cal_cw_x;
extern volatile s16 cw97_frame;
extern volatile s16 cw97_x;
extern volatile s16 cw97_y;
extern volatile u32 cw97_img;
extern volatile u32 cw97_anim_tbl;
extern volatile u32 cw97_hdr;
extern volatile u32 cw97_attr;

extern volatile u8  cw98_prio;
extern volatile u8  cw98_anim_flag;
extern volatile u8  cw98_disp;
extern volatile u8  cw98_tpage;
extern volatile u8  cw100_anim_flag;
extern volatile s16 cw98_frame;
extern volatile s16 cw98_x;
extern volatile s16 cw98_y;
extern volatile u32 cw98_attr;
extern volatile u32 cw98_cels;
extern volatile u32 cw98_anim_tbl;
extern volatile u32 cw98_hdr;

void name_ent_kuchi_anif(void)
{
    s32 sel;
    s32 sum;
    s32 rating;
    s16 temp;
    u32 colA;
    u32 ptrA;
    u32 ptrB;
    u32 ptrC;

    if (get_CD_State() & 4) {
        sel = SD_CDLevelGet();
    } else {
        sel = 0;
    }

    name_ent_kuchi_level_buf[frame_counter & 3] = sel;
    sum = name_ent_kuchi_level_buf_3 + D_8014D194_a + name_ent_kuchi_level_buf_1 + name_ent_kuchi_level_buf_2;
    rating = (sum / 4) / 10;
    name_ent_kuchi_now = sel;
    if (rating >= 4) {
        rating = 3;
    }

    class_clear_n(97, rating);

    if (get_CD_State() & 4) {
        ptrA = 0x80180258;
        temp = cal_cw_x;
        colA = 0x41000000;
        ptrB = 0x80180298;
        ptrC = 0x8018024C;

        cw97_prio = 9;
        cw97_mode = 64;
        cw97_attr = colA;
        cw97_disp = 164;
        cw97_img = ptrA;
        cw97_anim_tbl = ptrB;
        cw97_hdr = ptrC;
        cw97_frame = rating + 6;
        cw97_sel = 128;
        cw97_x = temp;
        cw97_y = -120;

        cw98_prio = 9;
        cw98_anim_flag = 64;
        cw98_attr = colA;
        cw98_disp = 164;
        cw98_cels = ptrA;
        cw98_anim_tbl = ptrB;
        cw98_hdr = ptrC;
        cw98_frame = rating + 10;
        cw98_tpage = 128;
        cw98_x = temp;
        cw98_y = -120;

        cw100_anim_flag = 0;
    }
}
