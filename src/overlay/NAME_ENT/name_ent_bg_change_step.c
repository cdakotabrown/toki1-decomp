/* @SymbolStart: 0x80145A00 */
/* @SymbolSize: 992 */
/* @RodataBase: 0x8014C950 */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=4 */
/* @PostProc: swap_instructions --offset1 0x314 --offset2 0x318 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:a2 0x14:rs:v1:a2 0x14:rt:v1:a2 0x1c:rs:v1:a2 0x28:rt:v1:a2 0x70:rt:t8:a2 0x74:rs:t8:a2 0x74:rt:t8:a2 0x7c:rs:t8:a2 0x7c:rt:t9:t8 0x84:rt:t9:t8 0x90:rt:t0:t9 0x94:rs:t0:t9 0xa4:rt:t2:a2 0xa8:rs:t2:a2 0xa8:rt:t2:a2 0xac:rt:t1:v1 0xb4:rt:t1:v1 0xbc:rs:t2:a2 0xbc:rt:t3:t0 0xc0:rt:t3:t0 0xe4:rt:v0:v1 0xe8:rs:v0:v1 0xe8:rt:v0:v1 0xec:rs:v1:a2 0xec:rt:t4:t1 0xf0:rs:v0:v1 0xf8:rs:v0:v1 0xf8:rt:v0:v1 0x100:rt:v0:v1 0x108:rs:v0:v1 0x118:rt:t4:t1 0x154:rt:v0:v1 0x158:rs:v0:v1 0x158:rt:v0:v1 0x160:rs:v0:v1 0x16c:rt:v0:v1 0x174:rs:v0:v1 0x178:rt:v0:v1 0x17c:rs:v0:v1 0x17c:rt:v0:v1 0x184:rs:v0:v1 0x184:rt:v0:v1 0x18c:rt:v0:v1 0x190:rt:t5:v1 0x19c:rt:t5:v1 0x1a0:rt:v0:v1 0x1a4:rs:v0:v1 0x1a4:rt:v0:v1 0x1ac:rs:v0:v1 0x1c4:rt:t6:a2 0x1c8:rs:t6:a2 0x1c8:rt:t6:a2 0x1d0:rs:t6:a2 0x1d0:rt:t7:t2 0x1d8:rt:t7:t2 0x1dc:rt:v0:v1 0x1e0:rs:v0:v1 0x1e0:rt:v0:v1 0x1e4:rs:v1:a2 0x1e4:rt:t8:t3 0x1e8:rs:v0:v1 0x1f0:rs:v0:v1 0x1f0:rt:v0:v1 0x1f8:rt:v0:v1 0x200:rs:v0:v1 0x210:rt:t8:t3 0x22c:rt:t9:t4 0x230:rs:t9:t4 0x258:rt:t0:a2 0x25c:rs:t0:a2 0x25c:rt:t0:a2 0x264:rs:t0:a2 0x264:rt:t1:t5 0x268:rt:t1:t5 0x284:rt:t2:t6 0x288:rs:t2:t6 0x29c:rt:t3:t7 0x2a4:rt:t3:t7 0x2b4:rt:t4:t8 0x2b8:rt:t4:t8 0x2c0:rt:t5:t9 0x2c4:rt:t5:t9 0x2c8:rt:t6:t0 0x2cc:rs:t6:t0 0x2cc:rt:t6:t0 0x2d4:rt:t6:t0 0x2d8:rt:t7:t1 0x2dc:rs:t7:t1 0x2dc:rt:t7:t1 0x2e4:rt:t7:t1 0x2e8:rt:t8:t2 0x2ec:rs:t8:t2 0x2ec:rt:t8:t2 0x2f4:rt:t8:t2 0x304:rt:t9:t3 0x308:rt:t9:t3 0x310:rt:t0:t4 0x314:rt:t2:a2 0x318:rt:t0:t4 0x31c:rs:t2:a2 0x31c:rt:t2:a2 0x324:rt:t1:t5 0x328:rt:t1:t5 0x330:rs:t2:a2 0x330:rt:t3:t6 0x334:rt:t3:t6 0x35c:rt:t4:t7 0x360:rs:t4:t7 0x37c:rt:t5:t8 0x380:rs:t5:t8 0x380:rt:t5:t8 0x388:rs:t5:t8 0x3b4:rt:t6:t9 0x3bc:rt:t6:t9 0x3c4:rt:t7:t0 0x3c8:rt:t7:t0 0x3d0:rt:t8:t1 0x3d4:rt:t8:t1 */
/* Address: 0x80145A08 (cross-prologue head absorbed from 0x80145A00) */
/* Size: 992 bytes (248 insns incl. hoisted head) */
#include "types.h"

extern void value_cache_find_or_load(s32, s32);
extern void name_ent_serifu_set(void);
extern void dec_bg_show(s32);
extern s32  get_CD_State2(void);
extern s32  dec_bg_init(s32);
extern void set_tbg_bri(s32);
extern void name_ent_text_scroll(void);
extern void bg_change(s32);
extern void set_dec_bri(s32);
extern void cd_read_start2(s32, s32, s32);
extern void name_ent_clut_set(void);
extern void class_clear_f(void);
extern void memcpy(s32, s32, s32);
extern void indexed_byte_lsb_set(s32, s32);
extern void dec_bg_reset(void);
extern void sub_step_up(void);

extern u8  step_mode;
extern volatile u8  name_ent_serifu_no;
extern volatile s32 name_ent_step_cnt;
extern u8  cal_cw_prio;
extern u8  cal_cw_mode;
extern u8  cal_cw_disp;
extern u8  cal_cw_sel;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s16 cal_cw_anim;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s32 cal_cw_hdr;
extern s32 cal_cw_attr;
extern s32 dec_bg_handle;

void name_ent_bg_change_step(void)
{
    s32 n;

    switch (step_mode) {
    case 0:
        value_cache_find_or_load(18790, 1);
        name_ent_serifu_no = 2;
        name_ent_step_cnt = 0;
        name_ent_serifu_set();
        dec_bg_show(1);
        step_mode++;
        break;
    case 1:
        if (get_CD_State2() & 1) {
            dec_bg_init(1);
            name_ent_step_cnt = 128;
            step_mode++;
            set_tbg_bri(128);
        }
        dec_bg_show(1);
        name_ent_text_scroll();
        break;
    case 2:
        n = name_ent_step_cnt;
        if (n >= 17) {
            n = n - 3;
            name_ent_step_cnt = n;
            set_tbg_bri(n & 0xFF);
        } else {
            step_mode = step_mode + 1;
        }
        bg_change(1);
        name_ent_text_scroll();
        break;
    case 3:
        name_ent_text_scroll();
        bg_change(1);
        break;
    case 4:
        bg_change(1);
        n = name_ent_step_cnt;
        if (n < 128) {
            name_ent_step_cnt = n;
            set_tbg_bri(n & 0xFF);
            n = name_ent_step_cnt;
            n = n + 4;
            name_ent_step_cnt = n;
        } else {
            n = 128;
            name_ent_step_cnt = n;
            set_tbg_bri(128);
            n = name_ent_step_cnt;
            set_dec_bri(n & 0xFF);
            cd_read_start2(49, 0x801A4000, 0xAB18);
            step_mode++;
        }
        break;
    case 5:
        n = name_ent_step_cnt;
        if (n >= 3) {
            n = n - 1;
            name_ent_step_cnt = n;
            set_dec_bri(n & 0xFF);
        } else {
            step_mode = step_mode + 1;
        }
        dec_bg_show(1);
        break;
    case 6:
        if (get_CD_State2() & 1) {
            name_ent_clut_set();
            class_clear_f();
            cd_read_start2(17, 0x80180000, 0xAC3F);
            step_mode++;
        }
        dec_bg_show(1);
        break;
    case 7:
        if (get_CD_State2() & 1) {
            memcpy(0x801F0000, 0x80180000, 0x8800);
            cal_cw_prio = 10;
            cal_cw_mode = 0;
            cal_cw_attr = 0x41000000;
            cal_cw_disp = 36;
            cal_cw_img = 0x801800D8;
            cal_cw_anim_tbl = 0x801800F0;
            cal_cw_hdr = 0x801800D4;
            cal_cw_anim = 0;
            cal_cw_sel = 128;
            cal_cw_x = -160;
            cal_cw_y = -120;
            step_mode++;
            value_cache_find_or_load(0xACDD, 1);
        }
        dec_bg_show(1);
        break;
    case 8:
        if (get_CD_State2() & 1) {
            indexed_byte_lsb_set(1, dec_bg_init(1));
            if (dec_bg_handle == -1) {
                dec_bg_reset();
                value_cache_find_or_load(0xACDD, 1);
            } else {
                sub_step_up();
                cal_cw_disp = 164;
                cal_cw_x = -160;
                cal_cw_y = -120;
            }
        }
        dec_bg_show(1);
        break;
    }
}
