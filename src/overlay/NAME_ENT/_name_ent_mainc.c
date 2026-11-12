/* @SymbolStart: 0x80148E50 */
/* @SymbolSize: 528 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x30:rs:v0:v1 0x38:rs:v0:v1 0x5c:rt:t6:v1 0x60:rs:t6:v1 0x60:rt:t6:v1 0x68:rs:t6:v1 0x68:rt:t7:t6 0x70:rt:t7:t6 0x7c:rt:t8:t7 0x80:rs:t8:t7 0x94:rt:t9:v1 0x98:rs:t9:v1 0x98:rt:t9:v1 0xa0:rs:t9:v1 0xa0:rt:t0:t8 0xa8:rt:t0:t8 0xb4:rt:t1:t9 0xb8:rs:t1:t9 0xc8:rt:t2:v1 0xcc:rs:t2:v1 0xcc:rt:t2:v1 0xd4:rs:t2:v1 0xd4:rt:t3:t0 0xdc:rt:t3:t0 0xe8:rt:t4:t1 0xec:rs:t4:t1 0xf0:rt:t5:t2 0xf4:rs:t5:t2 0xf4:rt:t5:t2 0xfc:rs:t5:t2 0xfc:rt:t6:t3 0x100:rs:t6:t3 0x11c:rt:t7:t4 0x120:rs:t7:t4 0x120:rt:t7:t4 0x128:rs:t7:t4 0x174:rt:t8:t5 0x17c:rt:t8:t5 0x184:rt:t9:t6 0x188:rt:t9:t6 0x190:rt:t0:t7 0x194:rt:t0:t7 0x19c:rt:t1:t8 0x1a0:rt:t1:t8 0x1a4:rt:t2:t9 0x1a8:rs:t2:t9 0x1a8:rt:t2:t9 0x1b0:rt:t2:t9 0x1b4:rt:t3:t0 0x1b8:rs:t3:t0 0x1b8:rt:t3:t0 0x1c0:rt:t3:t0 0x1c4:rt:t4:t1 0x1c8:rs:t4:t1 0x1c8:rt:t4:t1 0x1d0:rt:t4:t1 0x1d8:rt:t5:t2 0x1dc:rt:t5:t2 0x1e4:rt:t6:t3 0x1e8:rt:t6:t3 0x1f0:rt:t7:t4 0x1f4:rt:t7:t4 0x1fc:rt:t8:t5 0x204:rt:t8:t5 */
/* _name_ent_mainc - NAME_ENT overlay, Address: 0x80148E58, Size: 520 bytes (130 insns) */
#include "types.h"

extern u8  step_mode;
extern u8  cal_cw_mode;
extern s32 dec_bg_handle;
extern u8  cw97_prio;
extern u8  cw97_mode;
extern s32 cw97_attr;
extern u8  cw97_disp;
extern s32 cw97_img;
extern s32 cw97_anim_tbl;
extern s32 cw97_hdr;
extern s16 cw97_frame;
extern u8  cw97_sel;
extern s16 cw97_x;
extern s16 cw97_y;

extern void cd_read_start2(s32, s32, s32);
extern s32  get_CD_State2(void);
extern void value_cache_find_or_load(s32, s32);
extern s32  dec_bg_init(s32);
extern void indexed_byte_lsb_set(s32, s32);
extern s32  dec_bg_reset(void);
extern void class_clear_f(void);
extern void name_ent_bg_set(s32, s32);
extern void memcpy(s32, s32, s32);
extern void sub_step_up(void);
extern void dec_bg_show(s32);
extern u8 step_mode;

void _name_ent_mainc(void)
{
    s32 t;

    switch (step_mode) {
    case 0:
        cd_read_start2(49, (s32)0x801A4000, 0xAB49);
        step_mode = step_mode + 1;
        break;
    case 1:
        if (get_CD_State2() & 0x0001) {
            cd_read_start2(9, (s32)0x801F0000, 0xAC91);
            step_mode = step_mode + 1;
        }
        break;
    case 2:
        if (get_CD_State2() & 0x0001) {
            value_cache_find_or_load(0xAD0B, 1);
            step_mode = step_mode + 1;
        }
        break;
    case 3:
        if (get_CD_State2() & 0x0001) {
            if ((cal_cw_mode & 0x0001) == 0) {
                t = dec_bg_init(1);
                indexed_byte_lsb_set(1, t);
                if (dec_bg_handle == -1) {
                    dec_bg_reset();
                    value_cache_find_or_load(0xAD0B, 1);
                } else {
                    class_clear_f();
                    name_ent_bg_set((s32)0x801A4000, 15);
                    memcpy((s32)0x80180000, (s32)0x801F0000, 18432);
                    cw97_prio = 9;
                    cw97_mode = 64;
                    cw97_attr = 0x41000000;
                    cw97_disp = 164;
                    cw97_img = (s32)0x801800F4;
                    cw97_anim_tbl = (s32)0x80180114;
                    cw97_hdr = (s32)0x801800F0;
                    cw97_frame = 4;
                    cw97_sel = 128;
                    cw97_x = -160;
                    cw97_y = -120;
                    sub_step_up();
                }
            }
        }
        break;
    }

    dec_bg_show(1);
}
