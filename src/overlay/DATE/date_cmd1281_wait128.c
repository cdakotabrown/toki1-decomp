/* @SymbolStart: 0x8014FD20 */
/* @SymbolSize: 112 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x44:rt:v0:v1 0x48:rs:v0:v1 0x48:rt:v0:v1 0x50:rs:v0:v1 0x50:rt:t7:v0 0x54:rs:t7:v0 0x54:rt:t8:v0 0x58:rs:v0:v1 0x58:rt:t6:v1 0x60:rs:t8:v0 0x64:rt:t6:v1 */
/* Function: date_cmd1281_wait128 */
/* Overlay: DATE */
/* Address: 0x8014FD20 (cross-prologue head) / 0x8014FD28 (body) */
/* Size: 112 bytes (28 instructions) */
#include "types.h"

extern void SD_SeqIdClear(void);
extern void device_cmd_send(s32 a);
extern void don_init(void);
extern void sub_sub_step_up(void);

extern volatile s32 step_counter;

void date_cmd1281_wait128(void)
{
    s32 v;
    s32 isMatch;

    v = step_counter;
    if (v == 0) {
        SD_SeqIdClear();
        device_cmd_send(177);
        device_cmd_send(1281);
        don_init();
        v = step_counter;
    }
    isMatch = (u32)(v ^ 0x80) < 1U;
    step_counter = v + 1;
    if (isMatch != 0) {
        sub_sub_step_up();
    }
}
