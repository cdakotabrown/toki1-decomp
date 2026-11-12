/* @SymbolStart: 0x80142A50 */
/* @SymbolSize: 152 */
/* geko_sasoi_device_load -- GEKO overlay, Address: 0x80142A88 (true start 0x80142A50) */
/* Size: 152 bytes (38 insns) = 56-byte cross-prologue head + 96-byte body. */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_load_start(s32, u32, u32, s32, s32, s32);
extern s32 device_load_poll(void);

extern u8 schedule_cursor;
extern u8 schedule_kind[];
extern s32 girl_head_tbl[];
extern s32 girl_body_tbl[];
extern s32 girl_seq_tbl[];

void geko_sasoi_device_load(void)
{
    device_load_start(1, 0xBFA6, 0xBF87,
                  girl_head_tbl[schedule_kind[schedule_cursor * 4]],
                  girl_body_tbl[schedule_kind[schedule_cursor * 4]],
                  girl_seq_tbl[schedule_kind[schedule_cursor * 4]]);

    if (device_load_poll() == 1) {
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
