/* @SymbolStart: 0x80136bfc */
/* @SymbolSize: 228 */
/* kangei_choice_bg_set - KANGEI overlay, Address: 0x80136C10, Size: 208 bytes (52 insns) */
/* @CompileFlags: -Wo,-notail */
/* @PostProc: patch_insn_reg_field 0x20:rt:v1:v0 0x28:rt:a0:v1 0x2c:rs:a0:v1 0x2c:rt:v1:v0 0x44:rt:a1:a0 0x48:rs:a1:a0 0x48:rt:v1:v0 0x60:rt:v1:v0 0x64:rs:v1:v0 0x64:rt:v1:v0 0x6c:rs:v1:v0 0x74:rs:v1:v0 0x74:rt:a1:a0 0x7c:rs:v1:v0 0x7c:rt:a0:v1 0x84:rs:v1:v0 */
#include "types.h"

extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

extern u8 kangei_choice_no;
extern u8 kangei_choice_tbl;

extern volatile u8 kangei_choice_bg_no;

void kangei_choice_bg_set(void)
{
    u8 mode;
    u8 idx;
    u8 sel;

    idx = kangei_choice_no;
    sel = *(&kangei_choice_tbl + (s32)idx);

    if (sel == 2) {
        dec_bg_cache_step(17075);
        goto tail;
    }
    if (sel == 1) {
        dec_bg_cache_step(18301);
        goto tail;
    }
    mode = kangei_choice_bg_no;
    switch (mode) {
        case 0:
            dec_bg_cache_step(18311);
            goto tail;
        case 1:
            dec_bg_cache_step(18318);
            goto tail;
        case 2:
            dec_bg_cache_step(18327);
            goto tail;
        case 3:
            dec_bg_cache_step(18345);
            goto tail;
        default:
            dec_bg_cache_step(18354);
            break;
    }
tail:
    sub_sub_step_up();
}
