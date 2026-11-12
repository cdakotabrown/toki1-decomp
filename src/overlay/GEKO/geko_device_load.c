/* @SymbolStart: 0x80135B78 */
/* @SymbolSize: 132 */
/* geko_device_load -- GEKO overlay, Address: 0x80135B88, Size: 116 bytes, Semi-matched */
#include "types.h"
extern u8 cdq_footer_chk;
extern u8 schedule_buf;

extern void sub_sub_step_up(void);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);
extern void device_port_init(s32 a0, s32 a1);

void geko_device_load(void)
{
    s32 t6;

    cdq_footer_chk = 0;
    t6 = schedule_buf;

if (t6 == 64) {
        device_port_init(0, 512);
    }

    device_load_start(1, 0xBFA6, 0xBF87, 0xCAA3, 0xCA5D, 0xCA4C);

    if (device_load_poll() == 1) {
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
