/* @SymbolStart: 0x80139F84 */
/* @SymbolSize: 480 */
/* @PostProc: swap_instructions --offset1 0xE8 --offset2 0xE0 */
/* @PostProc: swap_instructions --offset1 0xE0 --offset2 0xDC */
/* @PostProc: swap_instructions --offset1 0xDC --offset2 0xD8 */
/* @PostProc: swap_instructions --offset1 0xD8 --offset2 0xD4 */
/* @PostProc: swap_instructions --offset1 0xD4 --offset2 0x40 */
/* @PostProc: patch_insn_reg_field 0x28:rt:v0:t7 0x2c:rs:v0:t7 0x2c:rt:v0:t7 0x34:rs:v0:t7 0x34:rt:t7:t8 0x3c:rs:t7:t8 0x44:rs:v0:t7 0x44:rt:t8:t9 0x50:rs:t8:t9 0x58:rs:v0:t7 0x58:rt:t9:t0 0x64:rs:t9:t0 0x6c:rs:v0:t7 0x6c:rt:t0:t1 0x78:rs:t0:t1 0x80:rs:v0:t7 0x80:rt:t1:t2 0x8c:rs:t1:t2 0x94:rs:v0:t7 0x94:rt:t2:t3 0xa0:rs:t2:t3 0xa8:rs:v0:t7 0xa8:rt:t3:t4 0xb4:rs:t3:t4 0xb8:rt:t4:t5 0xbc:rs:t4:t5 0xbc:rt:t4:t5 0xc4:rs:t4:t5 0xc4:rt:t5:t6 0xcc:rs:t5:t6 0xd4:rt:t6:t8 0xd8:rs:t6:t8 0xd8:rt:t6:t8 0xe0:rs:t6:t8 0xe0:rt:t7:t9 0xe8:rt:t7:t9 0x10c:rt:t8:t0 0x114:rt:t8:t0 0x11c:rt:t9:t1 0x120:rt:t9:t1 0x128:rt:t0:t2 0x12c:rt:t0:t2 0x134:rt:t1:t3 0x138:rt:t2:t7 0x13c:rs:t2:t7 0x13c:rt:t2:t7 0x140:rt:t1:t3 0x148:rt:t3:t4 0x14c:rs:t3:t4 0x14c:rt:t3:t4 0x150:rt:t2:t7 0x158:rt:t4:t5 0x15c:rs:t4:t5 0x15c:rt:t4:t5 0x160:rt:t3:t4 0x168:rt:t5:t6 0x16c:rs:t5:t6 0x16c:rt:t5:t6 0x170:rt:t4:t5 0x178:rt:t5:t6 0x190:rt:t6:t8 0x194:rt:t6:t8 0x19c:rt:t7:t9 0x1a0:rt:t7:t9 0x1a8:rt:t8:t0 0x1ac:rt:t8:t0 0x1b4:rt:t9:t1 0x1b8:rt:t9:t1 0x1c8:rt:t0:t2 0x1cc:rt:t0:t2 */
/* shugaku_cw97_set2 - SHUGAKU overlay, Address: 0x80139F8C (true entry 0x80139F84) */
#include "types.h"
#include "tokimeki/class_w.h"

extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void class_clear_n(s32 a0);
extern void sub_sub_step_up(void);

extern u8   shugaku_place;
extern u8   now_sub_sub_step;

extern s32 *volatile shugaku_cw97_res;
extern s32  shugaku_cw97_res_img;
extern s32 *shugaku_cw97_res_anim_tbl;
extern s16  shugaku_cw97_res_aux;
extern s32  shugaku_cw97_res_pal;

extern u8   cw97_prio;
extern u8   cw97_mode;
extern u8   cw97_disp;
extern u8   cw97_sel;
extern u8   cw97_rate;
extern u8   cw97_bright;
extern s32  cw97_img;
extern s32  cw97_anim_tbl;
extern s16  cw97_w14;
extern s16  cw97_anim;
extern s16  cw97_frame;
extern s16  cw97_x;
extern s16  cw97_y;
extern s32  cw97_hdr;
extern s32  cw97_attr;
extern u8   cw97_clut;
extern CLASS_W class_w[];

s32 shugaku_cw97_set2(void)
{
    s32 *p;

    if (shugaku_place == 1) {
        p = shugaku_cw97_res;
        if (p[0] == (s32)0x801E2120 && p[1] == (s32)0x801E6120 &&
            p[2] == (s32)0x801EA120 && p[3] == (s32)0x801EE120 &&
            p[4] == (s32)0x801F2120 && p[5] == (s32)0x801F6120 &&
            p[6] == (s32)0x801FA120 && *shugaku_cw97_res_anim_tbl == (s32)0x801E20A8) {
            goto ok;
        }
        now_sub_sub_step = (u8)(now_sub_sub_step - 2);
        return 0;
    ok:
        palette_upload_enqueue(shugaku_cw97_res_pal, 17, 0, 1, 0);
        class_clear_n(97);
        cw97_prio = 12;
        cw97_mode = 1;
        cw97_attr = 0x01000000;
        cw97_disp = 132;
        cw97_img = shugaku_cw97_res_img;
        cw97_anim_tbl = (s32)shugaku_cw97_res_anim_tbl;
        cw97_hdr = (s32)shugaku_cw97_res;
        cw97_w14 = shugaku_cw97_res_aux;
        cw97_anim = 0;
        cw97_frame = 0;
        cw97_rate = 1;
        cw97_clut = 17;
        cw97_x = 16;
        cw97_y = -119;
        cw97_bright = 0;
        cw97_sel = 1;
        class_w[97].cel_mode = 0;
    }
    sub_sub_step_up();
}
