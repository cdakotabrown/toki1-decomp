/* @SymbolSize: 124 */
/* @PostProc: patch_stack_complete --old 88 --new 96 */
/* @PostProc: remap_stack_slots --from 32 --to 40 */
/* @PostProc: patch_insn_reg_field 0x18:rt:v1:v0 0x28:rs:v1:v0 0x68:rs:v1:v0 0x6c:rt:v0:t9 0x74:rs:v0:t9 */
/* ending_calendar_main - ENDING overlay, Address: 0x80133F2C, Size: 124 bytes (31 insns) */
#include "types.h"

typedef struct {
    s32 data[13];
} DispatchTable13;

extern DispatchTable13 ending_main3_tbl;
extern u8 now_sub_sub_step;

void ending_calendar_main(void)
{
    u8  t1;
    DispatchTable13 tbl;

    void (*fn)(s32);

    tbl = ending_main3_tbl;

    t1 = now_sub_sub_step;
    fn = (void (*)(s32))tbl.data[t1];
    fn(128);
}
