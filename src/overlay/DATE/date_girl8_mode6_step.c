/* date_girl8_mode6_step - DATE overlay, Address: 0x8014ECE8, Size: 288 bytes (72 insns) */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic0 */
/* @PostProc: remap_stack_slots --from 44 --to 40 */
/* @PostProc: patch_insn_reg_field 0x18:rt:a2:v0 0x14:rt:a1:t6 0x1c:rs:a1:t6 0x1c:rt:v0:t6 0x24:rs:v0:t6 0x24:rt:a0:t9 0x28:rd:v1:t0 0x28:rs:a2:v0 0x2c:rs:v0:t6 0x2c:rt:t6:at 0x30:rs:v0:t6 0x30:rt:v0:t6 0x34:rs:v1:t0 0x34:rt:t6:at 0x38:rs:v0:t6 0x38:rt:t7:at 0x3c:rs:v1:t0 0x3c:rt:v1:t0 0x40:rs:v1:t0 0x40:rt:t7:at 0x44:rs:v0:t6 0x44:rt:t8:at 0x48:rs:v0:t6 0x48:rt:a0:t9 0x4c:rs:v1:t0 0x4c:rt:t8:at 0x50:rs:v0:t6 0x50:rt:t9:at 0x54:rt:t0:t1 0x58:rs:v1:t0 0x58:rt:t9:at 0x5c:rs:t0:t1 0x5c:rt:t0:t1 0x64:rd:t1:t2 0x64:rt:t0:t1 0x68:rd:t2:t3 0x68:rs:a2:v0 0x68:rt:t1:t2 0x6c:rs:t2:t3 0x6c:rt:a1:t9 0x74:rs:a1:t9 0x7c:rt:t3:t4 0x80:rs:t3:t4 0x80:rt:t3:t4 0x84:rt:t4:t5 0x88:rs:t3:t4 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rs:t4:t5 0x9c:rd:t5:t8 0xa0:rs:t5:t8 0xb0:rt:t6:t7 0xb4:rs:t6:t7 0xb4:rt:t6:t7 0xb8:rt:t7:t6 0xbc:rs:t6:t7 0xc4:rs:t7:t6 0xc4:rt:t7:t6 0xcc:rs:t7:t6 0xd4:rd:t8:t0 0xd8:rs:t8:t0 0xe8:rt:t9:t1 0xec:rs:t9:t1 0xec:rt:t9:t1 0xf0:rt:t0:t2 0xf4:rs:t9:t1 0xfc:rs:t0:t2 0xfc:rt:t0:t2 0x104:rs:t0:t2 0x10c:rd:t1:t3 0x110:rs:t1:t3 */
#include "types.h"

extern void device_cmd_send(s32);

extern u8 now_sub_sub_step;
extern s32 date_sprite_on;
extern s32 frame_counter;
extern u32 date_main6_fn_tbl[43];

void
date_girl8_mode6_step(void)
{
    u32 table[43];
    u32 *src;
    u32 *dst;
    u32 *end;
    u32 idx;
    void (*fp)(s32);

    src = (u32 *)((u32)date_main6_fn_tbl);
    end = src + 42;
    dst = table;
    do {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        src += 3;
        dst += 3;
    } while (src != end);
    dst[0] = src[0];

    idx = now_sub_sub_step;
    fp = (void (*)(s32))table[idx];
    fp(128);

    if (date_sprite_on != 0 && (u32)frame_counter % 180 == 0) {
        device_cmd_send(1281);
    }
    if (date_sprite_on != 0 && (u32)frame_counter % 210 == 10) {
        device_cmd_send(1282);
    }
    if (date_sprite_on != 0 && (u32)frame_counter % 450 == 20) {
        device_cmd_send(1280);
    }
}
