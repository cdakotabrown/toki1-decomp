/* @CompileFlags: -cckr */
/* @SymbolStart: 0x8013735C */
/* @SymbolSize: 768 */
/* @PostProc: patch_insn_reg_field 0x254:rt:t3:t5 0x25c:rs:t3:t5 0x25c:rt:t5:t6 0x264:rt:t5:t6 0x268:rt:t6:t7 0x26c:rs:t6:t7 0x26c:rt:t6:t7 0x274:rd:t7:t1 0x274:rt:t6:t7 0x278:rd:t1:t8 0x278:rt:t7:t1 0x27c:rs:t1:t8 0x2a8:rt:a0:t8 0x2ac:rs:a0:t8 0x2ac:rt:a0:t8 0x2b4:rd:t8:t9 0x2b4:rt:a0:t8 0x2b8:rd:t9:t4 0x2b8:rt:t8:t9 0x2bc:rs:t9:t4 0x2bc:rt:t4:t2 0x2c4:rs:t4:t2 0x2c8:rd:t2:t0 0x2cc:rt:t2:t0 0x2d0:rt:t0:t5 0x2d8:rs:t0:t5 0x2d8:rt:t3:t6 0x2dc:rt:t3:t6 */
#include "types.h"

extern void device_cmd_send();
extern void season_bgm_set();
extern void CDQ_init();
extern void device_state_init();
extern s32 device_load_poll();
extern void event_log_add();
extern void return_schedule();
extern u8 step_mode;
extern u32 step_counter;
extern u8 now_girl;
extern u8 now_month;
extern u8 girl2_basyo;
extern volatile u8 schedule_cursor;
typedef struct {
    u32 a : 9;
    u32 f : 3;
    u32 b : 20;
} Bits;
extern Bits girl8_flag;
extern u8 shougatu_date_girl;
extern u8 shougatu_date_kind;
extern u8 screen_w[];

s32 shougatu_schedule_return2()
{
    u8 sel;
    u8 idx;
    u32 w;
    s32 base;

    if (step_mode == 0) {
        device_cmd_send(207);
        season_bgm_set();
        step_mode++;
    }
    sel = step_mode;
    if (sel == 1) {
        if (step_counter++ >= 1024) {
            CDQ_init();
            device_state_init();
            step_mode = 0;
        }
        if (device_load_poll() == 1) {
            step_mode++;
        } else {
            return 0;
        }
    }
    now_girl = shougatu_date_girl;
    if (shougatu_date_girl == 2 && (shougatu_date_kind == 2 || shougatu_date_kind == 3) &&
        (girl2_basyo >> 4) == 3) {
        event_log_add(8, (u8)(shougatu_date_girl * 10 + 6));
    } else if (shougatu_date_girl == 4 && shougatu_date_kind == 0 && now_month >= 6 &&
               now_month < 10) {
        event_log_add(8, (u8)(shougatu_date_girl * 10 + 3));
    } else {
        event_log_add(8, (u8)(shougatu_date_girl * 10 + shougatu_date_kind));
    }
    base = (s32)screen_w;
    if (shougatu_date_girl == 10) {
        *(s16 *)(screen_w + screen_w[schedule_cursor * 4 + 1693] * 56 + 450) = 40;
    } else {
        *(s16 *)(screen_w + screen_w[schedule_cursor * 4 + 1693] * 56 + 450) = 50;
    }
    idx = screen_w[schedule_cursor * 4 + 1693];
    if (idx != 8) {
        screen_w[idx * 56 + 456] |= 2;
    } else {
        ((Bits *)(base + 904))->f = girl8_flag.f + 1;
        if (((Bits *)(base + 904))->f == 4) {
            screen_w[screen_w[schedule_cursor * 4 + 1693] * 56 + 456] |= 2;
        }
    }
    device_cmd_send(34);
    device_cmd_send(33);
    device_cmd_send(47);
    return_schedule(1);
}
