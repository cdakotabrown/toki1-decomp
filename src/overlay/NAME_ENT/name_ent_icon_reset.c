/* @SymbolStart: 0x8013BB24 */
/* @CompileFlags: -Wo,-loopunroll,1 */
/* @SymbolSize: 380 */
/* @PostProc: patch_stack_complete --old 48 --new 72 */
/* @PostProc: remap_stack_slots --from 24 --to 44 --from 40 --to 48 --from 44 --to 52 */
/* @PostProc: patch_insn_reg_field 0x30:rt:a1:a0 0x34:rt:a0:a1 0x40:rt:v1:v0 0x48:rs:v1:v0 0x50:rs:v1:v0 0x58:rs:v1:v0 0x68:rt:v0:v1 0x70:rs:v0:v1 0x78:rs:v0:v1 0x80:rs:v0:v1 0x90:rs:v1:v0 0x98:rs:v0:v1 0xa4:rt:v1:v0 0xac:rs:v1:v0 0xb4:rs:v1:v0 0xbc:rs:v1:v0 0xcc:rt:v0:v1 0xd4:rs:v0:v1 0xdc:rs:v0:v1 0xe4:rs:v0:v1 0xf4:rs:v1:v0 0xfc:rs:v0:v1 */
/* Address: 0x8013BB2C, Size: 372 bytes (93 insns) */
#include "types.h"

extern void printf(u8 *a0, s32 a1);
extern void icon_init(void);
extern void icon_disp_switch(s32 a0);

extern s16 icon_xy[];
extern u8  name_ent_icon_err_fmt;
extern u8  name_ent_icon_zero_fmt;
extern u8  icon_mode;

void name_ent_icon_reset(void)
{
    s32 oob;
    s32 zeros;
    s16 *p;
    s16 *end;
    s16 x;
    s16 y;

    oob = 0;
    zeros = 0;

    if (icon_mode == 2) {
        end = &icon_xy[32];
        p = icon_xy;
        do {
            x = p[0];
            if (x >= 125) {
                oob++;
            }
            if (x < -134) {
                oob++;
            }
            y = p[1];
            if (y < -84) {
                oob++;
            }
            if (y >= 85) {
                oob++;
            }
            if (x == 0 && y == 0) {
                zeros++;
            }
            x = p[2];
            if (x >= 125) {
                oob++;
            }
            if (x < -134) {
                oob++;
            }
            y = p[3];
            if (y < -84) {
                oob++;
            }
            if (y >= 85) {
                oob++;
            }
            if (x == 0 && y == 0) {
                zeros++;
            }
            p += 4;
        } while (p != end);
    }

    printf(&name_ent_icon_err_fmt, oob);
    printf(&name_ent_icon_zero_fmt, zeros);

    if (zeros >= 2) {
        if (oob != 0) {
            s32 saved;
            saved = icon_mode;
            icon_mode = 0;
            icon_init();
            icon_disp_switch(0);
            icon_mode = (u8)saved;
        }
    }
}
