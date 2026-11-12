/* date_fn_dispatch2 -- DATE overlay, Address: 0x8014DC74 */
/* @PostProc: patch_stack_complete --old 192 --new 208 */
/* @PostProc: remap_stack_slots --map '36:48' */
/* @PostProc: patch_insn_reg_field 0x14:rt:v0:v1 0x24:rs:v0:v1 0x24:rt:v0:v1 0x58:rt:v0:v1 0x60:rt:v1:v0 0x6c:rs:v1:t3 0x6c:rt:t3:v0 0x8c:rs:v1:t8 0x8c:rt:t8:v0 0xa4:rs:v1:v0 0xc8:rt:v0:v1 0xcc:rs:v0:v1 0xcc:rt:v0:v1 0xd4:rs:v0:v1 0xd8:rs:v0:v1 0xe4:rt:v0:v1 0xe8:rs:v0:v1 0xe8:rt:v0:v1 0xf0:rs:v0:v1 0xf8:rs:v0:v1 */
#include "types.h"

typedef struct {
    s32 w[39];
} Table156;

extern Table156 date_fn_dispatch2_tbl;
extern volatile u8 now_sub_sub_step;
extern u8  cal_cw_disp;
extern s32 date_sprite_on;

extern void basyo_string2_step(void);
extern void wipe_main2(void);
extern s32 date_palette_cycle10(void);
extern s32 date_box_disp(void);
extern s32 date_palette_cycle7(void);

s32 date_fn_dispatch2(void)
{
    Table156 buf;
    s32 v1;
    s32 (*fp)(s32);

    v1 = now_sub_sub_step;
    buf = date_fn_dispatch2_tbl;
    fp = (s32 (*)(s32))buf.w[v1];
    if (fp == (s32 (*)(s32))&basyo_string2_step) {
        cal_cw_disp |= 0x80;
    }
    if (fp == (s32 (*)(s32))&wipe_main2) {
        cal_cw_disp &= ~0x80;
    }
    fp(128);
    if (date_sprite_on != 0) {
        date_palette_cycle10();
    }
    v1 = now_sub_sub_step;
    if (v1 == 14) {
        date_box_disp();
        v1 = now_sub_sub_step;
    }
    if (v1 < 25U) goto Lexit;
    if (v1 >= 36U) goto Lexit;
    date_palette_cycle7();
Lexit:
    ;
}
