/* @PostProc: patch_stack_complete --old 48 --new 64 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x20:rs:v0:v1 0x2c:rs:v0:v1 0x34:rs:v0:v1 0x38:rt:v0:v1 0x40:rt:v0:v1 0x48:rd:v0:v1 0x50:rt:v0:v1 0x54:rt:v0:v1 0x58:rt:v1:v0 0x5c:rs:v1:v0 0x5c:rt:v1:v0 0x64:rs:v1:v0 0x70:rs:v1:v0 0x78:rs:v1:v0 0x84:rs:v0:v1 0x84:rt:v0:v1 0x88:rs:v0:v1 0x88:rt:v0:v1 0x94:rt:v0:v1 0x98:rd:v1:v0 0x9c:rs:v1:v0 0xa0:rt:v1:v0 0xbc:rt:v1:a0 0xc4:rd:t0:v1 0xc4:rt:v1:a0 0xc8:rs:t0:v1 0xc8:rt:t1:v1 0xcc:rs:t1:v1 0xd8:rs:v1:a0 */
/* @SymbolStart: 0x801463F0 */
/* @SymbolSize: 212 */
/* Address: 0x801463F8 (window starts 0x801463F0 - absorbed cross-prologue head) */
/* Size: 212 bytes (53 instructions, head included) */
#include "types.h"

extern s32 printf(u8 *, s32);
extern s32 rand(void);

extern u8 now_year;
extern u8 now_month;
extern s32 etc_hosei_tbl[];
extern u8 etc_hosei_fmt[];

s32 get_etc_hosei(void)
{
    s32 *p;
    s32 av;
    s32 ret;
    s32 sel;

    switch (now_year) {
    case 95:
        sel = 0;
        break;
    case 96:
        sel = 3;
        break;
    case 97:
        sel = 6;
        break;
    default:
        sel = 6;
        break;
    }

    switch (now_month) {
    case 12:
        sel += 1;
        break;
    case 7:
        break;
    case 2:
        sel -= 1;
        break;
    }

    p = &etc_hosei_tbl[sel];
    printf(etc_hosei_fmt, *p);
    ret = rand();
    av = *p;
    if (((ret & 7) + av - 8) > 0) {
        return av;
    }
    return 0;
}
