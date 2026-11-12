/* @SymbolSize: 192 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x30:rt:v1:v0 0x34:rs:v1:v0 0x34:rt:v1:v0 0x3c:rs:v1:v0 0x44:rs:v1:v0 0x4c:rs:v1:v0 0x50:rs:v0:v1 0xbc:rt:t0:a0 0xc4:rt:t0:a0 */
#include "types.h"

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 name_ent_upper_col;
extern s32 name_ent_upper_row;
extern s32 name_ent_tab_no;

void name_ent_cell_set(void)
{
    s32 col;
    s32 mode;

    if (cursor_cw_x >= -119 && cursor_cw_x < 152 &&
        cursor_cw_y >= -47 && cursor_cw_y < 48) {
        col = (cursor_cw_x + 120) / 16;
        name_ent_upper_row = (cursor_cw_y + 48) / 16;
    } else {
        col = -1;
        name_ent_upper_row = -1;
    }
    mode = name_ent_tab_no;
    if (mode != 2) {
        name_ent_upper_col = col;
        if (mode != 3) {
            return;
        }
    }
    name_ent_upper_col = col;
    if (col == 16) {
        col = -1;
        name_ent_upper_col = col;
    }
}
