/* @PostProc: patch_insn_reg_field   0x14:rt:v0:a0 0x20:rt:v0:a0 0x30:rt:v0:a0 0x54:rt:v1:t9 0x5c:rs:v1:t9 0x5c:rt:t9:t0 0x64:rt:t9:t0 */
/* @SymbolStart: 0x80141CA4 */
/* @SymbolSize: 104 */
/* Address: 0x80141CB0 */
/* Size: 92 bytes (23 insns) */
#include "types.h"
extern u8 girl_no;

extern u8 ev_flag52;
extern u8 now_girl;

extern u8 screen_w[];

extern void date_name_set(void);
extern void sub_sub_step_up(void);

void geko_girl_set(void)
{
    u8 v;
    s32 idx;
    u8 flags;
    u8 *p;

    v = girl_no;
    ev_flag52 = v;
    now_girl = v;
    date_name_set();
    idx = now_girl;
    p = &screen_w[idx * 56 + 456];
    flags = *p;
    *p = flags & 0xFFFD;
    sub_sub_step_up();
}
