/* @SymbolStart: 0x80138BA8 */
/* @SymbolSize: 236 */
/* Address: 0x80138BB0 (window starts at the absorbed cross-prologue head */
/* Size: 236 bytes (59 insns) including the head */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32  device_load_poll(void);

extern s32 work_tarao;
extern s32 event_girl_no;

extern s32 girl_body_tbl[];
extern s32 girl_seq_tbl[];
extern s32 girl_head_tbl[];

void geko_event_girl_device_load(void)
{
    if (event_girl_no == 8 || (event_girl_no == 11 && work_tarao == 2)) {
        device_load_start(1, 0xC071, 0xC053,
                      girl_head_tbl[event_girl_no], girl_body_tbl[event_girl_no],
                      girl_seq_tbl[event_girl_no]);
    } else {
        device_load_start(1, 0xC051, 0xC02B,
                      girl_head_tbl[event_girl_no], girl_body_tbl[event_girl_no],
                      girl_seq_tbl[event_girl_no]);
    }

    if (device_load_poll() == 1) {
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
