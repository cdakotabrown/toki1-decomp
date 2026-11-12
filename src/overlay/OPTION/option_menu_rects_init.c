/* Address: 0x80132628, Size: 152 bytes (38 instructions) */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: scheduler_normalizer --const-fp --fence 33-33,35-35 */
/* @PostProc: patch_insn_reg_field 0x10:rd:v0:v1 0x14:rt:a1:a0 0x18:rd:v1:a1 0x1c:rt:a3:t0 0x20:rt:t0:a3 0x28:rt:a0:v0 0x2c:rs:v1:a1 0x34:rs:v1:a1 0x38:rs:v1:a1 0x3c:rs:v0:v1 0x3c:rt:a0:v0 0x40:rs:v0:v1 0x44:rs:v0:v1 0x44:rt:a1:a0 0x4c:rs:v0:v1 0x50:rs:v0:v1 0x50:rt:t0:a3 0x54:rs:v0:v1 0x58:rs:v0:v1 0x58:rt:a1:a0 0x5c:rs:v0:v1 0x60:rs:v1:a1 0x68:rs:v1:a1 0x6c:rs:v1:a1 0x70:rs:v0:v1 0x74:rs:v0:v1 0x74:rt:a0:v0 0x78:rs:v0:v1 0x78:rt:a1:a0 0x80:rs:v0:v1 0x84:rs:v0:v1 0x84:rt:t0:a3 0x88:rs:v0:v1 0x8c:rs:v0:v1 0x8c:rt:a1:a0 0x90:rs:v0:v1 0x94:rs:v1:a1 0x94:rt:v1:a1 0x98:rs:v1:a1 0x98:rt:a3:t0 0x9c:rs:v0:v1 0x9c:rt:v0:v1 */
#include "types.h"

void option_menu_rects_init(s16 *base)
{
    s16 *p;
    s32 x;

    p = base;
    x = 0;
    do {
        if (x < 160) {
            p[0] = -32;
            p[16] = (s16)(x - 70);
            p[32] = 80;
            p[48] = 20;
        } else {
            p[0] = 48;
            p[16] = (s16)(x - 230);
            p[32] = 80;
            p[48] = 20;
        }
        if (x < 140) {
            p[17] = (s16)(x - 50);
            p[1] = -32;
            p[33] = 80;
            p[49] = 20;
        } else {
            p[1] = 48;
            p[17] = (s16)(x - 210);
            p[33] = 80;
            p[49] = 20;
        }
        x += 40;
        p += 2;
    } while (x != 320);
}
