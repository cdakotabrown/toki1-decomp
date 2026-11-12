/* Address: 0x8004F690 */
/* Size: 252 bytes (63 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 48 --new 80 */
/* @PostProc: remap_stack_slots --map "36:52" */
/* @PostProc: patch_insn_reg_field 0x34:rd:a2:v1 0x40:rs:a2:v1 0x40:rt:a1:v0 0x48:rs:a1:v0 0x4c:rs:a1:v0 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x1c */
/* @PostProc: swap_branch_operands --offsets 0xd8 */
/* `@PostProc: custom_script custom_func_8004F690.py`, now retired and deleted. */
#include "types.h"

extern s16 check_end_k_tbl0;
extern u16 load_string_tbl0;
extern u16 load_string_tbl1[];
extern u16 load_string_tbl2;
extern u8 load_string_str0[];

extern void printf(u8 *);
extern void vload_font(s32, s32);

s32 load_string(u8 *arg0) {
    u16 *s0 = &load_string_tbl2;
    u8 *s1 = arg0;
    s32 saved = check_end_k_tbl0;
    u8 hi;
    u8 lo;
    u32 v1;
    u8 *q;
    u8 *r;
    s32 diff;
    s32 i;

loop_1:
    q = s1;
    s1++;
    hi = *q;
    r = s1;
    s1++;
    if (hi == 0) {
        goto exit1;
    }
    lo = *r;
    if (lo != 0) {
        goto next;
    }
exit1:
    return check_end_k_tbl0 - saved;
next:
    v1 = lo + (hi << 8);
    if ((u32)(u16)v1 < (u32)load_string_tbl0) {
        printf(load_string_str0);
        return check_end_k_tbl0 - saved;
    }
    i = 0;
    do {
        if ((u32)(u16)v1 < (u32)load_string_tbl1[i]) {
            diff = (s32)(u16)v1 - (s32)(u16)load_string_tbl1[i - 2];
            vload_font((s32)(u16)load_string_tbl1[i - 1] + diff, diff);
            goto loop_1;
        }
        i += 2;
    } while (s0 != &load_string_tbl1[i]);
    goto loop_1;
}
