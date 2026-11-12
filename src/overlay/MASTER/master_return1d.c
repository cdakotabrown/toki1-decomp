/* @SymbolStart: 0x80137B5C */
/* @SymbolSize: 152 */
/* master_return1d - MASTER overlay, Address: 0x80137B64, Size: 144 bytes (36 insns) */
#include "types.h"

extern void device_cmd_send(s32);
extern void season_bgm_set(void);
extern void sub_sub_step_up(void);
extern s32  device_load_poll(void);
extern void class_clear_n(s32);
extern void shared_system_state_reset(void);
extern void set_dec_bri(s32);
extern void schedule_cursor_set(void);
extern void return_schedule(s32);
extern void common_disp(void);

extern u8 day_flag;
extern u8 now_sub_sub_step;

void master_return1d(void) {
    s32 hidden_t6;

    hidden_t6 = now_sub_sub_step;

    if (hidden_t6 == 0) {
        device_cmd_send(191);
        season_bgm_set();
        sub_sub_step_up();
        goto end;
    }

    if (device_load_poll() == 1) {
        class_clear_n(96);
        day_flag |= 0x10;
        shared_system_state_reset();
        set_dec_bri(0);
        schedule_cursor_set();
        return_schedule(1);
    }

end:
    common_disp();
}
