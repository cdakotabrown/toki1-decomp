/* @SymbolStart: 0x8013E5F4 */
/* @SymbolSize: 196 */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic1 */
/* @PostProc: patch_stack_complete --old 48 --new 56 */
/* @PostProc: patch_insn_reg_field 0x28:rt:a0:a1 0x2c:rt:a1:a2 0x30:rs:a0:a1 0x30:rt:a0:a1 0x34:rs:a1:a2 0x34:rt:a1:a2 0x38:rt:v1:a0 0x3c:rt:a2:a3 0x40:rs:a0:a1 0x4c:rs:v1:a0 0x4c:rt:a2:a3 0x54:rs:a1:a2 0x58:rt:a0:a1 0x5c:rt:a0:a1 0x60:rt:a0:a1 0x64:rs:v1:a0 0x64:rt:v1:a0 0x68:rt:v1:a0 0x6c:rd:v1:a0 0x70:rs:v1:a0 0x78:rs:a0:a1 0x88:rt:v1:a0 0x90:rt:a0:v1 0x94:rs:v1:a0 0x94:rt:a2:a3 0x98:rs:v1:a0 0x98:rt:v1:a0 0x9c:rt:v1:a0 0xa0:rd:v1:a0 0xa4:rs:v1:a0 0xac:rs:a1:a2 0xb8:rt:a0:v1 */
/* geko_particle_fade_out2 -- GEKO overlay, Address: 0x8013E5FC, Size: 188 bytes (47 insns) */
#include "types.h"

extern s32 date_fade_out(void);
extern u8 fade_bri;
extern u8 now_sub_sub_step;
extern u8 Sprites[];
extern u8 now_sub_sub_step;

void geko_particle_fade_out2(void)
{
    s32 t6 = now_sub_sub_step;

    short i;
    u8 v;

    date_fade_out();
    v = fade_bri;
    for (i = 4; i < 19; i++) {
        if ((u32)v < 112U) {
            Sprites[i * 36 + 22] = v;
            Sprites[i * 36 + 21] = v;
            Sprites[i * 36 + 20] = v;
        }
    }
    if (t6 != now_sub_sub_step) {
        for (i = 4; i < 19; i++) {
            *(s32 *)&Sprites[i * 36] =
                *(s32 *)&Sprites[i * 36] | 0x80000000;
        }
    }
}
