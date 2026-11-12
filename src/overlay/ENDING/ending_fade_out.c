/* @SymbolStart: 0x80139F64 */
/* @SymbolSize: 208 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:a1 0x14:rs:v1:a1 0x14:rt:a0:a1 0x24:rs:a0:a1 0x34:rt:v1:a1 0x3c:rs:v1:a1 0x3c:rt:a0:a1 0x44:rs:a0:a1 0x44:rt:v0:a1 0x48:rs:v0:a1 0x48:rt:v1:t6 0x4c:rs:v1:t6 0x54:rd:v0:a1 0x54:rs:v1:t6 0x60:rt:v1:t6 0x64:rd:v0:a1 0x6c:rt:v0:a1 0x74:rt:v1:a1 0x7c:rs:v1:a1 0x80:rd:t6:t7 0x88:rs:t6:t7 0x90:rd:t7:t8 0x94:rt:t7:t8 0xa4:rt:v1:a1 0xa8:rs:v1:a1 0xa8:rt:v1:a1 0xb0:rt:v1:a1 0xb4:rd:t9:t0 0xb8:rs:t9:t0 0xb8:rt:v0:t1 0xbc:rd:t1:t2 0xbc:rt:v0:t1 0xc0:rd:t2:t3 0xc0:rt:v0:t1 0xc4:rd:t3:t4 0xc4:rs:t1:t2 0xc4:rt:t2:t3 0xcc:rs:t3:t4 0xcc:rt:v0:t1 */
/* ending_fade_out - ENDING overlay, Address: 0x80139F64, Size: 208 bytes (52 instructions) */
#include "types.h"

extern void device_cmd_send();
extern void sprite_rgb_set();
extern void ending_fade_disp();
extern void sub_sub_step_up();
extern u8  sprite4_r;
extern s32 event_girl_no;

void ending_fade_out(void)
{
    s32 a1 = sprite4_r;
    s32 s0;
    u32 t6;
    u8  a;
    s32 rgba;

    if (a1 == 128) {
        device_cmd_send(209);
        event_girl_no = 0;
        a1 = sprite4_r;
    }
    a1 -= 4;
    t6 = (u32)a1 & 0xFF;
    a1 = t6;
    if (t6 < 5u) {
        sprite4_r = t6;
        sub_sub_step_up();
        a1 = 0;
    }
    sprite4_r = (u8)a1;
    for (s0 = 4; s0 < 10; s0 = (s16)(s0 + 1)) {
        sprite_rgb_set((s32)(s16)s0, (s32)sprite4_r);
    }
    a    = sprite4_r;
    a    = (u8)(((u32)a << 6) >> 7);
    rgba = (s32)(((u32)a << 16) | ((u32)a << 8) | (u32)a);
    ending_fade_disp(rgba);
}
