/* @SymbolStart: 0x801323A8 */
/* @SymbolSize: 136 */
/* Address: 0x801323A8 (cross-prologue head) / 0x801323D0 (prologue) */
/* Size: 136 bytes (34 instructions), head included */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_load_start(s32 a0, s32 a1, s32 a2, s32 a3, s32 s0, s32 s1);
extern s32 device_load_poll(void);

extern u8 now_girl;
extern s32 girl_head_tbl[];
extern s32 girl_body_tbl[];
extern s32 girl_seq_tbl[];

void kangei_girl_device_load(void)
{
    s32 idx = now_girl;

    device_load_start(1, 0xBFA6, 0xBF87, girl_head_tbl[idx],
                  girl_body_tbl[idx], girl_seq_tbl[idx]);
    if (device_load_poll() == 1) {
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
