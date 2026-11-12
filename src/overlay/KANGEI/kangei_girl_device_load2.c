/* @SymbolStart: 0x8013426C */
/* @SymbolSize: 144 */
/* kangei_girl_device_load2 -- KANGEI overlay, Address: 0x80134294 (real entry 0x8013426C) */
/* Size: 144 bytes (36 insns) = 40-byte cross-prologue head + 104-byte body. */
#include "types.h"

extern void device_load_start(s32, u32, u32, s32, s32, s32);
extern s32 device_load_poll(void);
extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);

extern u8 now_girl;
extern s32 girl_head_tbl[];
extern s32 girl_body_tbl[];
extern s32 girl_seq_tbl[];

void kangei_girl_device_load2(void)
{
    s32 idx;
    s32 result;

    idx = now_girl;

    device_load_start(1, 0xBFA6, 0xBF87, girl_head_tbl[idx], girl_body_tbl[idx],
                  girl_seq_tbl[idx]);

    result = device_load_poll();
    if (result == 1) {
        device_cmd_send(512);
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
