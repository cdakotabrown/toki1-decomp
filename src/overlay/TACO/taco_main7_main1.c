/* @SymbolStart: 0x80137D10 */
/* @SymbolSize: 276 */
/* @PostProc: patch_insn_reg_field 0x40:rt:t6:a0 0x44:rs:t6:a0 0x44:rt:t6:a0 0x4c:rs:t6:a0 0x60:rt:t7:t6 0x64:rs:t7:t6 0x64:rt:t7:t6 0x68:rt:t9:v1 0x6c:rs:t9:v1 0x6c:rt:t9:v1 0x74:rs:t7:t6 0x74:rt:t8:t7 0x78:rt:t8:t7 0x80:rs:t9:v1 0x80:rt:t0:t8 0x88:rt:t0:t8 0xac:rt:t1:t9 0xb0:rs:t1:t9 0xb4:rt:t2:v1 0xb8:rs:t2:v1 0xb8:rt:t2:v1 0xc0:rs:t2:v1 0xc0:rt:t3:t0 0xc8:rt:t3:t0 0xd4:rt:t4:t1 0xd8:rs:t4:t1 0xec:rt:t5:t2 0xf0:rs:t5:t2 0xf0:rt:t5:t2 0xf4:rt:v0:t4 0xf8:rs:t5:t2 0xf8:rt:t6:t3 0xfc:rs:t6:t3 0x104:rs:v0:t4 0x104:rt:v0:t4 0x10c:rs:v0:t4 0x10c:rt:t7:t5 0x110:rt:t7:t5 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x30:rs:v0:v1 */
#include "types.h"

extern s16 taco_time_left;
extern u8 step_mode;
extern s32 frame_counter;
extern s16 mes_win_cw_y;

extern void taco_xa_sector_set(void);
extern void sub_sub_step_up(void);
extern void taco_mes_win_set(s32 a0);
extern s32 get_CD_State(void);
extern void dec_bg_show(s32 a0);

void taco_main7_main1(void)
{
    u8 sel = step_mode;

    s32 v0;
    s16 h;
    u8 b;
    s16 *pp;

    switch (sel) {
    case 0:
        if ((s16)taco_time_left < 3) {
            taco_xa_sector_set();
            h = taco_time_left;
            b = step_mode;
            taco_time_left = h + 1;
            step_mode = (u8)(b + 1);
        } else {
            sub_sub_step_up();
            taco_mes_win_set(2);
        }
        break;
    case 1:
        v0 = get_CD_State();
        if ((v0 & 4) != 0) {
            b = step_mode;
            step_mode = (u8)(b + 1);
        }
        break;
    case 2:
        v0 = get_CD_State();
        if ((v0 & 2) != 0) {
            step_mode = 0;
        }
        break;
    }

    dec_bg_show(2);
    pp = &mes_win_cw_y;
    if ((frame_counter & 1) != 0) {
        h = *pp;
        *pp = h - 1;
    }
}
