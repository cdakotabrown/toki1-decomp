/* @PostProc: patch_stack_complete --old 184 --new 192 */
/* func_80139604 - EVENT overlay, Address: 0x80139604, Size: 124 bytes (31 insns) */
/* @PostProc: remap_stack_slots --from 32 --to 40 */
/* @PostProc: patch_insn_reg_field 0x8:rt:v1:v0 0x18:rs:v1:v0 0x58:rs:v1:v0 0x5c:rt:v0:t9 0x64:rs:v0:t9 */
#include "types.h"

typedef struct {
    s32 w[3];
} Entry12;

typedef struct {
    Entry12 entries[12];
    s32 trailing;
} Table148;

extern Table148 D_801216C8;
extern u8 D_800B1AF6;

void func_80139604(void) {
    u8 idx;
    Table148 buf;
    void (*fn)(s32);

    buf = D_801216C8;
    idx = D_800B1AF6;
    fn = ((void (**)(s32))&buf)[idx];
    fn(128);
}
