/* @PostProc: patch_stack_complete --old 136 --new 144 */
/* func_801465AC - EVENT overlay, Address: 0x801465AC, Size: 124 bytes (31 insns) */
/* @PostProc: remap_stack_slots --from 32 --to 40 */
/* @PostProc: patch_insn_reg_field 0x8:rt:v1:v0 0x18:rs:v1:v0 0x58:rs:v1:v0 0x5c:rt:v0:t9 0x64:rs:v0:t9 */
#include "types.h"

typedef struct {
    s32 w[3];
} Entry12;

typedef struct {
    Entry12 entries[8];
    s32 trailing;
} Table100;

extern Table100 D_80123F30;
extern u8 D_800B1AF6;

void func_801465AC(void) {
    u8 idx;
    Table100 buf;
    void (*fn)(s32);

    buf = D_80123F30;
    idx = D_800B1AF6;
    fn = ((void (**)(s32))&buf)[idx];
    fn(128);
}
