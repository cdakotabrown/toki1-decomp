/* Address: 0x8013E2C0 */
/* Size: 308 bytes (77 instructions) */
/* @PostProc: patch_stack_complete --old 256 --new 264 */
/* @PostProc: patch_insn_reg_field 0x90:rd:v0:t9 0x94:rs:v0:t9 0x94:rt:v0:t9 0x9c:rs:v0:t9 0xbc:rt:t9:a3 0xc8:rd:v1:a0 0xcc:rs:t9:a3 0xcc:rt:t9:a3 0xd4:rs:t9:a3 0xe0:rt:a0:a3 0xe4:rt:a0:a3 0xec:rt:a0:a3 0xf8:rt:a0:a3 0x100:rs:v1:a0 0x104:rs:v1:a0 0x104:rt:v1:a0 0x108:rt:v1:a0 0x10c:rd:v1:a0 0x110:rs:v1:a0 0x11c:rt:a0:a3 0x120:rt:a0:a3 0x128:rt:a0:a3 0x130:rt:a0:a3 */
#include "types.h"

typedef struct { u32 w[53]; } Table53;
extern Table53 D_801225A8;

extern u8 D_800B1AF6;
extern s32 D_800B1AE4;

extern u16 D_80094714;
extern u16 D_80094718;

extern u8 D_8011F854;

extern u8 D_800EAFA7;
extern u8 D_800EAFEB;
extern u8 D_800F19AB;
extern u8 D_800EC190;

typedef void (*func_ptr_t)(s32);

extern void func_80078950();

void func_8013E2C0(void) {
    Table53 localTable;
    u8 charId;
    s16 j;
    func_ptr_t funcPtr;
    u8 brightness;
    u8 *basePtr;
    s32 offset;

    localTable = D_801225A8;

    func_80078950(&D_8011F854, D_800B1AF6, D_80094714, D_80094718);

    charId = D_800B1AF6;
    funcPtr = (func_ptr_t)localTable.w[charId];
    funcPtr(128);

    charId = D_800B1AF6;
    if (charId != 14) {
        return;
    }
    if (D_800B1AE4 & 1) {
        return;
    }

    brightness = D_800F19AB;
    if (brightness == 128) {
        brightness = 96;
    } else {
        brightness = 128;
    }

    D_800EAFA7 = brightness;
    D_800EAFEB = brightness;

    basePtr = &D_800EC190;
    for (j = 0; j < 6; j++) {
        offset = j * 36;
        basePtr[offset + 94] = brightness;
        basePtr[offset + 93] = brightness;
        basePtr[offset + 92] = brightness;
    }

    D_800F19AB = brightness;
}
