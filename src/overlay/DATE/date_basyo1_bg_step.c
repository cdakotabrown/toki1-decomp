/* Address: 0x8013705C (absorbed head)  Size: 356 bytes (89 instructions) */
/* @SymbolStart: 0x8013705C */
/* @SymbolSize: 356 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:v0 0x14:rs:v1:v0 0x5c:rt:v1:t6 0x60:rs:v1:t6 0x60:rt:v0:t6 0x64:rt:t7:t8 0x68:rd:t6:t7 0x68:rt:v0:t6 0x6c:rd:t7:t8 0x6c:rs:t7:t8 0x6c:rt:t6:t7 0x70:rs:t7:t8 0x70:rt:t7:t8 0x78:rs:t7:t8 0x80:rs:v0:t6 0x80:rt:t8:t9 0x88:rt:t8:t9 0xdc:rt:v1:t0 0xe0:rs:v1:t0 0xe0:rt:v0:t0 0xe4:rt:t0:t2 0xe8:rd:t9:t1 0xe8:rt:v0:t0 0xec:rd:t0:t2 0xec:rs:t0:t2 0xec:rt:t9:t1 0xf0:rs:t0:t2 0xf0:rt:t0:t2 0xf8:rs:t0:t2 0x100:rs:v0:t0 0x100:rt:t1:t3 0x108:rt:t1:t3 0x124:rt:v1:t4 0x128:rs:v1:t4 0x128:rt:v0:t4 0x12c:rt:t3:t7 0x130:rd:t2:t5 0x130:rt:v0:t4 0x134:rd:t3:t7 0x134:rs:t3:t7 0x134:rt:t2:t5 0x138:rs:t3:t7 0x138:rt:t3:t7 0x140:rs:t3:t7 0x148:rs:v0:t4 0x148:rt:t4:t8 0x150:rt:t4:t8 */
#include "types.h"

extern u8 date_pattern;
extern u8 now_month;
extern u8 rec_idx;
extern u8 rec_prev_val[];

extern void dec_bg_cache_step();
extern void event_log_add();
extern void sub_sub_step_up(void);

void date_basyo1_bg_step(void) {
    u8 mode;
    u32 s;
    u8 idx;

    mode = date_pattern;
    if (mode != 0) {
        goto L_0E8;
    }

    s = now_month;
    if (s < 3) {
        goto L_090;
    }
    if (s < 9) {
        goto L_0A0;
    }
L_090:
    dec_bg_cache_step(0x4318);
    goto L_0A8;
L_0A0:
    dec_bg_cache_step(0x430F);
L_0A8:
    idx = rec_idx;
    if (rec_prev_val[idx * 8] == 1) {
        rec_idx = idx - 1;
    }
    event_log_add(0, 0x25);
    goto L_1A8;

L_0E8:
    if (mode != 1) {
        goto L_168;
    }
    s = now_month;
    if (s < 3) {
        goto L_110;
    }
    if (s < 9) {
        goto L_120;
    }
L_110:
    dec_bg_cache_step(0x4306);
    goto L_128;
L_120:
    dec_bg_cache_step(0x42FD);
L_128:
    idx = rec_idx;
    if (rec_prev_val[idx * 8] == 1) {
        rec_idx = idx - 1;
    }
    event_log_add(0, 0x26);
    goto L_1A8;

L_168:
    dec_bg_cache_step(0x4322);
    idx = rec_idx;
    if (rec_prev_val[idx * 8] == 1) {
        rec_idx = idx - 1;
    }
    event_log_add(0, 0x27);

L_1A8:
    sub_sub_step_up();
}
