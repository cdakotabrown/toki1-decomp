/* Address: 0x80132490 (absorbed head)  Size: 136 bytes (34 instructions) */
/* @SymbolStart: 0x80132490 */
/* @SymbolSize: 136 */
#include "types.h"

extern u8 now_girl;
extern s32 girl_head_tbl[];
extern s32 girl_body_tbl[];
extern s32 girl_seq_tbl[];

extern void device_load_start();
extern s32 device_load_poll(void);
extern void sub_sub_step_up(void);

void girl_res_cd_read(void) {
    s32 i;

    i = now_girl;
    device_load_start(1, 0xC051, 0xC02B, girl_head_tbl[i], girl_body_tbl[i],
                  girl_seq_tbl[i]);
    if (device_load_poll() == 1) {
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
