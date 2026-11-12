/* @SymbolStart: 0x80132198 */
/* @SymbolSize: 392 */
/* @PostProc: swap_instructions --offset1 0xd0 --offset2 0xd4 */
/* @PostProc: patch_insn_reg_field 0xd0:rt:t7:v0 0xdc:rt:t7:v0 0xf4:rt:t8:t7 0xf8:rs:t8:t7 0x13c:rt:t9:v0 0x140:rs:t9:v0 0x140:rt:t9:v0 0x148:rs:t9:v0 0x148:rt:t0:t8 0x150:rt:t0:t8 0x168:rt:t1:t9 0x16c:rs:t1:t9 */
/* name_ent_cd_load - NAME_ENT overlay, Address: 0x801321A0, Size: 384 bytes (96 insns), Semi-matched */
#include "types.h"

extern u8 back_clear_g;
extern u8 back_clear_r;
extern u8 back_clear_b;
extern u8 vblank_flag;
extern u8 csr_speed;
extern u8 fade_bri;
extern u8 step_mode;

extern void text_sys_init();
extern void kanji_cell_width_set();
extern void draw2d3d();
extern void class_switch();
extern void prim_init();
extern void class_clear_f();
extern void cursor_init();
extern void set_flag_b3_high_bit();
extern void back_clear_switch();
extern void tpage_buf_clear();
extern void vram_area_clear();
extern void dec_bg_reset();
extern void indexed_byte_toggle();
extern void cd_read_start();
extern s32 get_CD_State2();
extern void dec_env_set();
extern void decdct();
extern void cd_read_start2();
extern void sub_sub_step_up();

void name_ent_cd_load(void)
{
    if (step_mode == 0) {

        text_sys_init();
        kanji_cell_width_set(16);
        draw2d3d(1, 0);
        class_switch(1);
        prim_init();
        class_clear_f();
        cursor_init(0);
        set_flag_b3_high_bit(0);
        back_clear_switch(1);
        back_clear_g = 0;
        back_clear_r = 0;
        back_clear_b = 0;
        tpage_buf_clear();
        vram_area_clear(0);
        vblank_flag = 0;
        dec_bg_reset();
        indexed_byte_toggle(2);
        cd_read_start(10, 0x80162000, 0xAA18);
        csr_speed = 3;
        fade_bri = 0;
        step_mode = 1;
    } else if (step_mode == 1) {

        if (get_CD_State2() & 1) {
            dec_env_set(0x80167000, 0x80188000, 0x80162000);
            decdct(320, 256, 320, 240);
            cd_read_start2(54, 0x80190000, 0xA9E2);
            step_mode = step_mode + 1;
        }
    } else if (step_mode == 2) {

        if (get_CD_State2() & 1) {
            cd_read_start2(6, 0x801E0000, 0x9DBF);
            sub_sub_step_up();
        }
    }
}
