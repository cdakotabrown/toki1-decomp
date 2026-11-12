/* @RodataBase: 0x8014C99C */
/* Address: 0x80146568 (cross-prologue head absorbed from 0x80146560) */
/* Size: 752 bytes (188 insns incl. hoisted head) */
/* @SymbolStart: 0x80146560 */
/* @SymbolSize: 752 */
/* @PostProc: patch_insn_reg_field 0x140:rt:t9:v0 0x148:rt:t9:v0 0x18c:rt:t0:t9 0x194:rt:t0:t9 0x19c:rt:t1:t0 0x1a4:rt:t1:t0 0x1b0:rt:t3:t2 0x1c0:rt:t2:t1 0x1cc:rt:t2:t1 0x1d0:rd:t4:t3 0x1d0:rs:t3:t2 0x1d8:rt:t4:t3 0x1f0:rt:t5:t4 0x1f4:rs:t5:t4 0x210:rt:t6:t5 0x218:rt:t6:t5 0x224:rt:t7:t6 0x228:rs:t7:t6 0x248:rt:t8:t7 0x250:rt:t8:t7 0x25c:rt:t9:t8 0x260:rs:t9:t8 0x288:rt:t0:t9 0x28c:rs:t0:t9 0x28c:rt:t0:t9 0x294:rs:t0:t9 */
#include "types.h"

extern void device_cmd_send(s32);
extern void value_cache_find_or_load(s32, s32);
extern s32  get_CD_State2(void);
extern s32  dec_bg_init(s32);
extern void indexed_byte_lsb_set(s32, s32);
extern void name_ent_bg_set(s32, s32);
extern void cd_read_start2(s32, s32, s32);
extern void dec_bg_reset(void);
extern void class_clear_f(void);
extern void memcpy(s32, s32, s32);
extern void name_ent_cal_cw_set(void);
extern void name_ent_cw99_set0(void);
extern void sub_step_up(void);
extern void dec_bg_show(s32);

extern volatile s32 name_ent_step_cnt;
extern s16 cw97_x;
extern u8  cw97_mode;
extern u8  cw97_disp;
extern u8  cw97_sel;
extern s16 cw97_tick;
extern s16 cw97_anim;
extern s16 cw97_frame;
extern u8  cw97_clut;
extern u8  step_mode;
extern s32 dec_bg_handle;

void name_ent_prologue_intro_load_body(void) {
    s32 n;
    s32 cond;

    switch (step_mode) {
    case 0:
        n = name_ent_step_cnt;
        if (n < 64) {
            n = n + 1;
        }
        cond = n < 128;
        n = n + 1;
        if (cond) {
            cw97_x = -32 - n;
            name_ent_step_cnt = n;
        } else {
            name_ent_step_cnt = n;
            device_cmd_send(1541);
            value_cache_find_or_load(0xACF6, 1);
            cw97_mode = 5;
            cw97_disp = 164;
            cw97_anim = 0;
            cw97_frame = 1;
            cw97_sel = 128;
            step_mode++;
        }
        break;
    case 1:
        if (cw97_frame == 3 && cw97_tick == 0) {
            device_cmd_send(1541);
        }
        if (cw97_mode & 1) {
            break;
        }
        if (get_CD_State2() & 1) {
            name_ent_step_cnt = 128;
            indexed_byte_lsb_set(1, dec_bg_init(1));
            name_ent_bg_set(0x801A4000, 15);
            cd_read_start2(49, 0x801A4000, 0xAA54);
            cw97_clut = 29;
            step_mode++;
        }
        break;
    case 2:
        n = name_ent_step_cnt;
        if (n < -32) {
            step_mode++;
        } else {
            n = n - 3;
            cw97_x = -32 - n;
            name_ent_step_cnt = n;
        }
        break;
    case 3:
        if (get_CD_State2() & 1) {
            value_cache_find_or_load(0xACD4, 1);
            step_mode++;
        }
        break;
    case 4:
        if (get_CD_State2() & 1) {
            cd_read_start2(25, 0x801F0000, 0xABCA);
            step_mode++;
        }
        break;
    case 5:
        if (get_CD_State2() & 1) {
            name_ent_bg_set(0x801A4000, 15);
            indexed_byte_lsb_set(1, dec_bg_init(1));
            if (dec_bg_handle == -1) {
                dec_bg_reset();
                value_cache_find_or_load(0xACD4, 1);
            } else {
                class_clear_f();
                memcpy(0x80180000, 0x801F0000, 0xC800);
                name_ent_cal_cw_set();
                name_ent_cw99_set0();
                sub_step_up();
            }
        }
        break;
    }
    dec_bg_show(1);
}
