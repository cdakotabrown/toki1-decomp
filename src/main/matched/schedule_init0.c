/* Address: 0x8006CDB4 */
/* Size: 192 bytes (48 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers_liveness --target-exe disks/SLPS_000.65 --address 0x8006CDB4 */
/* @PostProc: swap_registers_auto --swaps t0:t9 --exclude-offsets 0x9c,0xa4 --unidirectional */
#include "types.h"

extern u8 step_mode;
extern u8 schedule_cursor;
extern u8 schedule_buf[];
extern void restore_bgm(void);
extern void mes_window_switch(s32);
extern void last_date_spot_timer_dec(void);
extern s32 card_wait_check(void);
extern void sub_sub_step_up(void);

void schedule_init0(void) {
    u8 mode = step_mode;

    switch (mode) {
    case 0:
        if (schedule_buf[schedule_cursor * 4] != 64) {
            restore_bgm();
        }
        mes_window_switch(0);
        last_date_spot_timer_dec();
        step_mode = step_mode + 1;
        break;
    case 1:
        if (card_wait_check() != 0) {
            step_mode = step_mode + 1;
        }
        break;
    case 2:
        sub_sub_step_up();
        break;
    }
}
