/* @SymbolStart: 0x80134590 */
/* @SymbolSize: 132 */
/* geko_day_load_step -- GEKO overlay, Address: 0x801345A4, Size: 112 bytes */
#include "types.h"

extern u8 now_day;
extern u8 cal_cell_tbl[];

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32 a0);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);

void geko_day_load_step(void)
{
    if (cal_cell_tbl[now_day] == 5) {
        device_cmd_send(191);
    }

    device_load_start(1, 0xBFA6, 0xBF87, 0xCAA3, 0xCA5D, 0xCA4C);

    if (device_load_poll() == 1) {
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
