/* Address: 0x8014A948 (cross-prologue head absorbed), Size: 400 bytes (100 insns) */
/* @SymbolStart: 0x8014A948 */
/* @SymbolSize: 400 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rt:v1:v0 0x1c:rs:v0:v1 0x1c:rt:v0:v1 0x20:rs:v1:v0 0x20:rt:v1:v0 0x34:rs:v0:v1 0x44:rt:v0:v1 0x48:rt:v0:v1 0x5c:rs:v1:v0 0x6c:rt:v0:v1 0x70:rt:v0:v1 0x8c:rs:v0:v1 0x9c:rs:v0:v1 */
#include "types.h"

extern u8 *tt_player_wk;
extern u8 *tt_trail_buf;

extern void tt_sprite_uv_set(u8 *out, s32 kind);

void tt_sprite_pos_init(u8 *out, s32 sel)
{
    u8 *p;
    u8 *q;
    u8 *r;

    p = tt_player_wk;
    q = tt_trail_buf;
    if (sel < 6) {
        tt_sprite_uv_set(out, p[85]);
    } else {
        tt_sprite_uv_set(out, q[146]);
    }
    if (sel < 6) {
        *(s32 *)(out + 32) = *(s16 *)(p + 20) << 16;
        *(s32 *)(out + 36) = *(s16 *)(p + 22) << 16;
    } else if (sel < 8) {
        r = tt_trail_buf;
        *(s32 *)(out + 32) = *(s16 *)(r + 80) << 16;
        *(s32 *)(out + 36) = *(s16 *)(r + 112) << 16;
    } else if (sel < 10) {
        r = tt_trail_buf + 164;
        *(s32 *)(out + 32) = *(s16 *)(r + 80) << 16;
        *(s32 *)(out + 36) = *(s16 *)(r + 112) << 16;
    } else {
        r = tt_trail_buf + 328;
        *(s32 *)(out + 32) = *(s16 *)(r + 80) << 16;
        *(s32 *)(out + 36) = *(s16 *)(r + 112) << 16;
    }
    if (sel >= 2 && sel < 4) {
        *(s32 *)(out + 40) = 0xFFFD0000;
    } else if (sel >= 4 && sel < 6) {
        *(s32 *)(out + 40) = 0x00030000;
    } else {
        *(s32 *)(out + 40) = 0;
    }
    *(s32 *)(out + 44) = 0xFFFA0000;
}
