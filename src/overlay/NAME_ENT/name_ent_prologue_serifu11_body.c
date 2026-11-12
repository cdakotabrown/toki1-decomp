/* @SymbolStart: 0x801474CC */
/* @SymbolSize: 804 */
/* @RodataBase: 0x8014C9F8 */
/* @PostProc: patch_insn_reg_field 0x7c:rt:a0:v0 0x94:rs:a0:v0 0x94:rt:a0:v0 0x9c:rs:a0:v0 0xa4:rs:a0:v0 0xc4:rt:a0:v0 0xc8:rs:a0:v0 0xc8:rt:a0:v0 0xd0:rs:a0:v0 0xd8:rs:a0:v0 0xd8:rt:t5:t6 0xe0:rt:t5:t6 0xe8:rt:t5:t6 0xf4:rt:t5:t6 0x104:rt:t6:t7 0x108:rs:t6:t7 0x108:rt:t6:t7 0x110:rs:t6:t7 0x110:rt:t7:t8 0x114:rt:t7:t8 0x120:rt:t8:t9 0x124:rs:t8:t9 0x12c:rt:t9:t0 0x130:rt:t0:t1 0x134:rt:t0:t1 0x138:rt:t9:t0 0x15c:rt:t1:t2 0x160:rs:t1:t2 0x160:rt:t1:t2 0x168:rs:t1:t2 0x168:rt:t2:t3 0x170:rt:t2:t3 0x198:rt:t3:t4 0x19c:rs:t3:t4 0x19c:rt:t3:t4 0x1a4:rs:t3:t4 0x1a4:rt:t4:t5 0x1ac:rt:t4:t5 0x1b8:rt:t5:t6 0x1bc:rs:t5:t6 0x1d0:rt:t6:t7 0x1d4:rs:t6:t7 0x1d4:rt:t6:t7 0x1dc:rs:t6:t7 0x1dc:rt:t7:t8 0x1f0:rt:t8:t9 0x1f4:rs:t8:t9 0x268:rt:t9:t0 0x278:rt:t0:t1 0x284:rt:t1:t2 0x288:rt:t1:t2 0x290:rt:t2:t3 0x298:rt:t3:t4 0x29c:rs:t3:t4 0x29c:rt:t3:t4 0x2a8:rt:t4:t5 0x2ac:rs:t4:t5 0x2ac:rt:t4:t5 0x2b8:rt:t5:t6 0x2bc:rs:t5:t6 0x2bc:rt:t5:t6 0x2d4:rt:t6:t7 0x2e0:rt:t7:t8 0x2e4:rt:t7:t8 0x2f0:rt:a0:v0 0x2fc:rt:a0:v0 0x1e4:rt:t7:t8 0x270:rt:t9:t0 0x27c:rt:t0:t1 0x294:rt:t2:t3 0x2a4:rt:t3:t4 0x2b4:rt:t4:t5 0x2c4:rt:t5:t6 0x2d8:rt:t6:t7 */
/* Address: 0x801474D4, Size: 796 bytes (199 insns) */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern u8 step_mode;
extern u8 name_ent_serifu_no;
extern s32 name_ent_step_cnt;
extern s16 cal_cw_x;
extern s16 cw97_x;
extern s16 cw98_x;
extern s16 cal_cw_y;
extern u8 cal_cw_prio;
extern u8 cal_cw_mode;
extern u8 cal_cw_disp;
extern u8 cal_cw_bright;
extern u8 cal_cw_clut;
extern u32 cal_cw_attr;
extern u32 cal_cw_img;
extern u32 cal_cw_anim_tbl;
extern u32 cal_cw_hdr;
extern u16 cal_cw_frame;
extern LIBGS_BG Bg[];

extern void name_ent_serifu_set();
extern s32 get_CD_State();
extern void name_ent_text_scroll();
extern void name_ent_prologue_serifu11_kuchi_ani();
extern void value_cache_find_or_load();
extern s32 get_CD_State2();
extern void device_load_start();
extern void device_cmd_send();
extern s32 device_load_poll();
extern void cd_read_start2();
extern void class_clear_f();
extern void memcpy();
extern s32 dec_bg_init();
extern void indexed_byte_lsb_set();
extern void set_dec_bri();
extern void k_sub_reset();
extern void name_ent_bg_set();
extern void class_clear_n();
extern void name_ent_palette_init();
extern void device_port_init();
extern void sub_step_up();
extern void dec_bg_show();

void name_ent_prologue_serifu11_body(void)
{
    s32 sel;
    s32 v;

    sel = step_mode;
    switch (sel) {
        case 0:
            name_ent_serifu_no = 11;
            name_ent_serifu_set();
            step_mode = step_mode + 1;
            break;
        case 1:
            if (get_CD_State() & 4) {
                step_mode = step_mode + 1;
            }
            v = cal_cw_x;
            if (v < -160) {
                cal_cw_x = v + 1;
            }
            break;
        case 2:
            name_ent_text_scroll();
            name_ent_prologue_serifu11_kuchi_ani();
            v = cal_cw_x;
            if (v < -160) {
                cw97_x = v + 1;
                cw98_x = v + 1;
                cal_cw_x = v + 1;
            }
            break;
        case 3:
            value_cache_find_or_load(0xAD00, 1);
            step_mode = step_mode + 1;

        case 4:
            if (get_CD_State2() & 1) {
                device_load_start(1, 0, 0, 0xCCD9, 0xCC94, 0xCC90);
                device_cmd_send(5);
                device_cmd_send(177);
                step_mode = step_mode + 1;
            }
            break;
        case 5:
            if (device_load_poll() == 1) {
                cd_read_start2(49, 0x801A4000, 0xAAB6);
                step_mode = step_mode + 1;
            }
            break;
        case 6:
            if (get_CD_State2() & 1) {
                cd_read_start2(9, 0x801F0000, 0xABFC);
                step_mode = step_mode + 1;
            }
            break;
        case 7:
            if (get_CD_State2() & 1) {
                class_clear_f();
                memcpy(0x80180000, 0x801F0000, 18432);
                indexed_byte_lsb_set(1, dec_bg_init(1));
                set_dec_bri(0);
                k_sub_reset();
                name_ent_bg_set(0x801A4000, 14);
                Bg[0].g = 0;
                Bg[0].r = 0;
                Bg[0].b = 0;
                class_clear_n(96);
                cal_cw_prio = 12;
                cal_cw_mode = 64;
                cal_cw_attr = 0x41000000;
                cal_cw_disp = 132;
                cal_cw_img = 0x80180078;
                cal_cw_anim_tbl = 0x80180088;
                cal_cw_hdr = 0x80180074;
                cal_cw_frame = 0;
                cal_cw_clut = 14;
                cal_cw_y = -120;
                cal_cw_bright = 0;
                cal_cw_x = -160;
                name_ent_palette_init();
                name_ent_step_cnt = 0;
                device_port_init(1, 0);
                sub_step_up();
            }
            break;
    }

    dec_bg_show(1);
}
