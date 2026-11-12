/* Function: holiday_girl_init */
/* Address: 0x80077428 */
/* Size: 760 bytes (190 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v1:52 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_registers_liveness --address 0x80077428 */
/* @PostProc: patch_insn_reg_field 0x98:rt:v1:t4 0xa0:rt:v1:t4 0xa0:rd:t3:t6 0xa8:rs:t3:t6 0xcc:rd:t9:t2 0xd0:rt:t9:t2 */
#include "common.h"

extern u8 step_mode;
extern u8 schedule_cursor;
extern u8 screen_w[];
extern u8 sched_icon;
extern u8 byouki_kind;
extern u16 byouki_state;
extern s32 dec_bg_handle;
extern s32 work_fuku;
extern s32 holiday_girl_init_tbl0[];
extern s32 holiday_girl_init_tbl1[];

extern void text_sys_init(void);
extern void mes_window_switch(s32);
extern void icon_can_use_set(s32, s32);
extern s32 basyo_bg_load(void);
extern void indexed_byte_lsb_set(s32, s32);
extern s32 get_CD_State2(void);
extern s32 dec_bg_init(s32);
extern void dec_bg_reset(void);
extern void holiday_girl_pick(void);
extern void cd_read_start2(s32, s32, s32);
extern void sub_sub_step_up(void);

void holiday_girl_init(void) {
    s32 i;
    u8 *base;
    u32 mode;
    u32 idx;
    u32 sel;
    u32 cur;
    u32 v;
    u32 k;
    s32 result;

    text_sys_init();
    mes_window_switch(0);

    mode = step_mode;
    i = 0;

    if (mode == 0) {
        base = (u8 *)((u32)screen_w);

        do {
            if (i != *(u8 *)((u32)base + schedule_cursor * 4 + 1693)) {
                icon_can_use_set(i, 0);
            } else {
                icon_can_use_set(i, 1);
            }
            i++;
        } while (i != 14);

        idx = schedule_cursor;
        sel = *(u8 *)((u32)base + idx * 4 + 1693);
        cur = sched_icon;
        sched_icon = ((u32)sel << 4) | (cur & 0xff0f);
        cur = (u8)sched_icon;
        v = ((u32)cur >> 4) % 9;
        *(base + v + 309) = *(base + v + 309) + 1;

        if (((u32)sched_icon >> 4) == 4) {
            v = *(u16 *)(base + 244);
            k = v >> 12;
            if (k < 11) {
                ((u16 *)((u32)base + k * 4))[822] += 6;
            }
        }

        result = basyo_bg_load();
        if (result != -1) {
            if (result == 0 || result == 1) {
                indexed_byte_lsb_set(0, result);
                step_mode = 2;
                return;
            }
        } else {
            step_mode = 1;
            return;
        }
    } else if (mode == 1) {
        if (get_CD_State2() & 1) {
            result = dec_bg_init(0);
            indexed_byte_lsb_set(0, result);
            if (dec_bg_handle == -1) {
                dec_bg_reset();
                basyo_bg_load();
                return;
            } else {
                step_mode = 2;
                return;
            }
        }
    } else if (mode == 2) {
        holiday_girl_pick();

        if ((byouki_kind & 0xf) == 3) {
            if (!(byouki_state & 1)) {
                cd_read_start2(9, 0x801e0000, 0x7d8a);
                step_mode = 3;
                return;
            }
        }

        base = &screen_w[(u32)0 * idx];
        idx = schedule_cursor;
        sel = *(u8 *)((u32)base + idx * 4 + 1693);
        if (sel != 4) {
            cd_read_start2(41, 0x801e0000, holiday_girl_init_tbl1[work_fuku]);
            step_mode = 3;
            return;
        } else {
            cd_read_start2(41, 0x801e0000, holiday_girl_init_tbl0[work_fuku]);
            step_mode = 3;
            return;
        }
    } else if (mode == 3) {
        if (get_CD_State2() & 1) {
            sub_sub_step_up();
        }
    }
}
