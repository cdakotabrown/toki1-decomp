/* Function: step34_ffinit */
/* Address: 0x8005A7B8 */
/* Size: 320 bytes (80 instructions)  -> window is now 328 bytes / 82 insns */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @SymbolStart: 0x8005A7B0 */
/* @SymbolSize: 328 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_insn_reg_field 0x0:rt:v0:v1 0x4:rs:v0:v1 0x4:rt:v0:v1 0xc:rs:v0:v1 0x30:rt:t6:v1 0x34:rs:t6:v1 0x34:rt:t6:v1 0x3c:rs:t6:v1 0x3c:rt:t7:t6 0x44:rt:t7:t6 0x4c:rs:v0:v1 0x60:rt:t8:v1 0x64:rs:t8:v1 0x64:rt:t8:v1 0x6c:rs:t8:v1 0x6c:rt:t9:t7 0x74:rt:t9:t7 0x7c:rs:v0:v1 0x8c:rt:t0:t8 0x90:rs:t0:t8 0xc0:rt:t6:t4 0xc4:rt:t1:t9 0xc8:rs:t1:t9 0xc8:rt:t1:t9 0xd0:rt:t1:t9 0xd0:rd:t2:t0 0xd4:rs:t2:t0 0xd4:rt:t1:t9 0xd4:rd:t2:t0 0xd8:rt:t2:t0 0xd8:rd:t2:t0 0xdc:rt:t2:t0 0xe8:rt:t4:t2 0xf0:rt:t4:t2 0xfc:rt:t5:t3 0x104:rt:t5:t3 0x110:rt:t6:t4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 step_mode;
extern u8 now_year;
extern u8 now_month;
extern u8 menu_mode;
extern volatile u8 slot_no;
extern s16 cursor_work_copy_tbl0;
extern u8 anim_fade;
extern u8 palette_tbl[];

extern s32 device_load_poll(void);
extern void device_port_init(s32, s32);
extern void cd_read_start2(s32, u32, s32);
extern s32 get_CD_State2(void);
extern void cal_font_load(u8, u8, s32);
extern void mascot_init(s32);
extern void mascot_disp_switch(s32);
extern void sub_sub_step_up(void);

void step34_ffinit(void)
{
    s32 mode;
    s32 val;
    s16 *pval;

    mode = step_mode;
    if (mode == 0) {
        if (device_load_poll() == 1) {
            device_port_init(0, 0);
            step_mode = step_mode + 1;
        }
    } else if (mode == 1) {
        cd_read_start2(149, 0x801A0000, 0x9B94);
        step_mode = step_mode + 1;
    } else if (mode == 2) {
        if (get_CD_State2() & 1) {
            cal_font_load(now_year, now_month, 0);
            pval = &cursor_work_copy_tbl0;
            if ((u32)menu_mode < 4u) {
                menu_mode = palette_tbl[slot_no * 0xF00] & 3;
            } else if ((u32)menu_mode >= 4u) {
                menu_mode = menu_mode - 4;
            }
            val = *pval;
            anim_fade = 254;
            if (val < 14) {
                mascot_init(val);
                mascot_disp_switch(1);
            }
            sub_sub_step_up();
        }
    }
}
