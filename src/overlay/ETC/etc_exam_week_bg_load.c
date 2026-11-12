/* @SymbolStart: 0x801468E8 */
/* @SymbolSize: 316 */
/* Address: 0x801468F0 */
/* Size: 308 bytes (77 instructions) */
#include "types.h"

extern s32  device_load_poll(void);
extern void device_port_init(s32 a0, s32 a1);
extern s32  value_cache_find_or_load(s32 a0, s32 a1);
extern s32  set_dec_bri(s32 a0);
extern void indexed_byte_lsb_set(s32 a0, s32 a1);
extern void goto_sub_sub_step(s32 a0);
extern void sub_sub_step_up(void);
extern s32  device_port_get(void);
extern void dec_bg_show(s32 a0);

extern u8 bukatu_pt10[];
extern u8 schedule_cursor;

extern u8 now_youbi;

void etc_exam_week_bg_load(void)
{
    s32 v0;
    s32 ret;
    s32 idx;

    if (now_youbi != 0) {
        idx = schedule_cursor;
        if (bukatu_pt10[idx * 4] == 64) {
            goto block2;
        }
    }

    v0 = device_load_poll();
    if (v0 != 1) {
        return;
    }
    device_port_init(1, 517);
    ret = value_cache_find_or_load(16292, 0);
    set_dec_bri(0);
    if (ret == -1) {
        goto label994;
    }
    if (ret == 0) {
        goto label974;
    }
    if (ret != 1) {
        return;
    }
label974:
    indexed_byte_lsb_set(0, ret);
    set_dec_bri(128);
    goto_sub_sub_step(3);
    return;
label994:
    sub_sub_step_up();
    return;

block2:
    v0 = device_port_get();
    if (v0 != 0) {
        goto label9BC;
    }
    device_port_init(1, 517);
label9BC:
    ret = value_cache_find_or_load(16292, 0);
    if (ret == -1) {
        goto labelA0C;
    }
    if (ret == 0) {
        goto label9E4;
    }
    if (ret != 1) {
        return;
    }
label9E4:
    indexed_byte_lsb_set(0, ret);
    dec_bg_show(0);
    set_dec_bri(128);
    goto_sub_sub_step(3);
    return;
labelA0C:
    sub_sub_step_up();
    return;
}
