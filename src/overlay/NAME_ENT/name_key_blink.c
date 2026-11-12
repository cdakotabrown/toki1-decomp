/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=3840 */
/* @PostProc: swap_instructions --offset1 0xcc --offset2 0xd0 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x20:rt:a1:a3 0x2c:rd:v0:v1 0x38:rs:a1:a3 0x38:rt:a1:a3 0x40:rt:a1:a3 0x44:rt:a1:a3 0x50:rt:a0:t9 0x58:rs:a0:t9 0x60:rt:a3:a2 0x64:rs:a3:a2 0x64:rt:a3:a2 0x6c:rd:t9:t5 0x6c:rt:a3:a2 0x70:rs:t9:t5 0x90:rd:t5:a1 0x94:rt:t5:a1 0xa4:rt:a3:a2 0xa8:rs:a3:a2 0xa8:rt:a3:a2 0xbc:rt:a1:a2 0xcc:rs:a3:a2 0xcc:rt:a3:a2 0xd0:rs:a1:a3 0xd0:rt:a1:a3 0xe0:rt:a2:a0 0xe4:rs:a2:a0 0xe4:rt:a2:a0 0xe8:rt:v1:v0 0xf4:rs:v0:v1 0xf8:rt:a1:a3 0xfc:rt:a1:a3 0x108:rt:v1:v0 0x10c:rt:a0:t6 0x110:rd:t6:t7 0x110:rt:a3:a2 0x114:rs:a0:t6 0x11c:rs:t6:t7 0x120:rs:a2:a0 0x130:rd:t7:a1 0x134:rd:t8:t5 0x134:rt:t7:a1 0x138:rs:v1:v0 0x13c:rd:t9:t8 0x13c:rt:v1:v0 0x140:rs:v1:v0 0x144:rd:t9:t8 0x148:rs:t9:t8 0x14c:rd:t5:t9 0x150:rs:t8:t9 0x15c:rs:a2:a0 0x164:rs:a2:a0 0x168:rs:a2:a0 0x16c:rs:v0:v1 0x170:rd:t9:t8 0x170:rt:a1:a3 0x174:rd:t9:t8 0x174:rs:t9:t8 0x174:rt:a1:a3 0x178:rd:t9:t8 0x178:rt:t9:t8 0x17c:rd:t8:t9 0x17c:rt:t9:t8 0x180:rs:t8:t9 0x180:rt:v1:v0 0x184:rt:a0:t6 0x188:rd:t6:t7 0x188:rt:a3:a2 0x18c:rs:a0:t6 0x194:rs:t6:t7 0x198:rs:v1:v0 0x19c:rs:a2:a0 0x1ac:rd:t7:a1 0x1b0:rd:t9:t6 0x1b0:rt:t7:a1 0x1b8:rd:t5:t9 0x1c0:rd:t5:t9 0x1c4:rs:t5:t9 0x1c8:rd:t6:t5 0x1cc:rs:t9:t5 0x1d8:rs:a2:a0 0x1e0:rs:a2:a0 0x1e4:rs:a2:a0 0x1e8:rs:v1:v0 0x1e8:rt:v1:v0 0x1ec:rs:v1:v0 0x1f0:rs:a2:a0 0x1f0:rt:a2:a0 */
/* name_key_blink - NAME_ENT overlay, Address: 0x80139A7C, Size: 492 bytes (123 insns) */
#include "types.h"

typedef struct {
    s32 f0;
    s16 f4;
    s16 f6;
    s16 f8;
    s16 f10;
    s16 f12;
    s8  f14;
    s8  f15;
    s16 f16;
    s16 f18;
    u8  f20;
    u8  f21;
    u8  f22;
    s16 f24;
    s16 f26;
    s16 f28;
    s16 f30;
    s32 f32;
} Ent;

extern Ent Sprites[];
extern s32 name_ent_view_page;
extern u8  slot_no;
extern u8  album_chr[];
extern u32 frame_counter;

void name_key_blink(void)
{
    s32 phase;
    s32 i;
    u8 f;

    phase = name_ent_view_page % 3;
    for (i = 0; i < 13; i++) {
        if (phase == 0) {
            f = album_chr[(slot_no * 3840) + 1672 + i];
            if (f == 0) {
                Sprites[i].f0 = 0;
                Sprites[i].f14 = (((frame_counter >> 2) % 3) * 16)
                    + ((i / 8) * 48) + 128;
            } else {
                Sprites[i].f0 = (s32)0x80000000;
            }
        } else {
            Sprites[i].f0 = (s32)0x80000000;
        }
    }
}
