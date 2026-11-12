/* @SymbolSize: 368 */
/* @PostProc: patch_insn_reg_field 0x10:rt:a0:v0 0x14:rs:a0:v0 0x14:rt:a0:v0 0x24:rs:a0:v0 0x2c:rs:a0:v0 0x34:rt:v0:v1 0x38:rt:v1:a0 0x3c:rs:v1:a0 0x3c:rt:v1:a0 0x40:rs:v0:v1 0x40:rt:v0:v1 0x44:rs:v1:a0 0x48:rs:v1:a0 0x48:rt:v0:v1 0x50:rs:v0:v1 0x60:rt:v0:v1 0x64:rs:v0:v1 0x64:rt:v0:v1 0x68:rs:a0:v0 0x70:rt:v1:a0 0x74:rs:a0:v0 0x84:rt:v1:a0 0x94:rt:v0:v1 0xa0:rs:v0:v1 0xac:rt:v1:a0 0xb0:rs:a0:v0 0xc0:rs:a0:v0 0xc8:rt:v1:a0 0xd8:rt:v0:v1 0xe4:rs:v0:v1 0xec:rs:v1:a0 0xf8:rs:a0:v0 0x100:rs:a0:v0 0x108:rt:v1:a0 0x118:rt:v0:v1 0x124:rs:v0:v1 0x12c:rs:v1:a0 0x138:rs:a0:v0 0x140:rs:a0:v0 0x148:rt:v1:a0 0x150:rt:a0:v0 0x158:rs:a0:v0 0x158:rt:v0:v1 0x160:rs:a0:v0 0x164:rs:v0:v1 0x16c:rs:v1:a0 */
#include "types.h"

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 name_ent_tab_hit;
extern s32 name_ent_tab_y_tbl[];

void name_ent_tab_index_set(void)
{
    s32 i;
    s32 y;

    name_ent_tab_hit = -1;
    y = cursor_cw_y;
    for (i = 0; i < 5; i++) {
        if (cursor_cw_x >= -157 && cursor_cw_x < -128 &&
            name_ent_tab_y_tbl[i] < y && y < name_ent_tab_y_tbl[i] + 16) {
            name_ent_tab_hit = i;
        }
    }
}
