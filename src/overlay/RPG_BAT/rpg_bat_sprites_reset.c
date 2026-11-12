/* rpg_bat_sprites_reset - RPG_BAT overlay, Address: 0x8013E4D0, Size: 124 bytes */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x60 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rt:a3:v0 0x18:rs:a3:v0 0x18:rt:a3:v0 0x1c:rs:v0:v1 0x1c:rt:v0:v1 0x20:rt:a2:a3 0x24:rt:a1:a2 0x28:rt:a0:a1 0x2c:rt:v1:a0 0x30:rs:v0:v1 0x30:rt:v0:v1 0x34:rs:v0:v1 0x34:rt:a3:v0 0x38:rs:v0:v1 0x38:rt:v1:a0 0x3c:rs:v0:v1 0x3c:rt:a0:a1 0x40:rs:v0:v1 0x40:rt:a1:a2 0x44:rs:v0:v1 0x44:rt:a1:a2 0x48:rs:v0:v1 0x48:rt:a2:a3 0x50:rs:v0:v1 0x50:rt:a2:a3 0x54:rt:v0:v1 0x58:rt:a3:v0 0x5c:rs:a3:v0 0x5c:rt:a3:v0 0x60:rs:v0:v1 0x60:rt:v0:v1 0x64:rs:v0:v1 0x64:rt:v0:v1 0x68:rs:v0:v1 0x68:rt:v1:a0 0x6c:rs:v0:v1 0x6c:rt:a0:a1 0x70:rs:v0:v1 0x70:rt:a1:a2 0x74:rs:v0:v1 0x74:rt:a1:a2 0x78:rs:v0:v1 0x78:rt:a2:a3 0x7c:rs:v0:v1 0x7c:rt:a3:v0 0x80:rs:v0:v1 0x80:rt:a2:a3 */
#include "types.h"

extern u8 cw40_active[];
extern u8 cw64_active[];
extern u8 cw29_active[];
extern u8 cw31_active[];

void rpg_bat_sprites_reset(void)
{
    u8 *p;

    p = cw40_active;
    do {
        p += 68;
        p[6461] = 5;
        *(u32 *)(p + 6516) = 0x41000000;
        p[6462] = 1;
        p[6466] = 1;
        *(u16 *)(p + 6486) = 4096;
        *(u16 *)(p + 6488) = 4096;
    } while (p < cw64_active);

    p = cw29_active;
    do {
        p += 68;
        p[6461] = 5;
        *(u32 *)(p + 6516) = 0x41000000;
        p[6462] = 1;
        p[6466] = 1;
        *(u16 *)(p + 6486) = 4096;
        *(u16 *)(p + 6488) = 4096;
    } while (p != cw31_active);
}
