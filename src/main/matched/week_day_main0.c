/* Function: week_day_main0 */
/* Address: 0x80074710 */
/* Size: 316 bytes (79 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x8007470C */
/* @SymbolSize: 320 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots 28:40 */
/* @PostProc: swap_instructions 0x20 0x28 */
/* @PostProc: swap_registers v0:v1 --only 0x28,0x2c,0x34 */
/* @PostProc: swap_registers t0:v0 --only 0xf0,0xf8 */
/* @PostProc: swap_registers t2:t1 --only 0xf4,0xfc */
/* @PostProc: swap_registers t3:t2 --only 0x108,0x10c,0x114 */
#include "types.h"

extern void device_cmd_send(s32 arg0);
extern void parameter_up_down(s32 arg0, s32 arg1, s32 arg2);
extern void parameter_show_init(void);
extern void sub_step_up(void);
extern void CDQ_init(void);

extern u8 cal_cw_mode;
extern s16 cal_cw_tick;
extern s16 cal_cw_anim;

extern u8 schedule_cursor;
extern u8 screen_w[];
extern u16 basyo_pack;
extern s32 step_counter;

void week_day_main0(void) {
    s32 saved_a0;
    u8 *ptr;
    s32 val;
    s32 cond;

    if (cal_cw_mode & 1) {
        goto check_limit;
    }

    if (cal_cw_tick == 0) {
        val = cal_cw_anim;
        switch (val & 1) {
        case 0:
            device_cmd_send(1281);
            break;
        case 1:
            device_cmd_send(1282);
            break;
        }

        ptr = screen_w + ((u32)schedule_cursor << 2);
        saved_a0 = ptr[1693];
        parameter_up_down(saved_a0, 0, ptr[1694]);
        parameter_show_init();

        if (saved_a0 == 4) {
            u32 rem;
            rem = ((u32)basyo_pack >> 12) % 11U;
            *(u16 *)(screen_w + (rem << 2) + 1644) += 1;
        }
    }

    val = cal_cw_tick;
    cond = (val < 13) ^ 1;
    cal_cw_tick = (s16)(val + 1);
    if (cond) {
        sub_step_up();
    }

check_limit:
    if ((u32)step_counter >= 601U) {
        CDQ_init();
        sub_step_up();
    }
}
