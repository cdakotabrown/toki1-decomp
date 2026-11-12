/* @SymbolSize: 160 */
/* @PostProc: patch_insn_reg_field 0x18:rt:v0:v1 0x1c:rs:v0:v1 0x28:rt:v1:a0 0x2c:rs:v1:a0 0x2c:rt:v1:a0 0x30:rt:a1:v1 0x34:rs:v1:a0 0x34:rt:a1:v1 0x38:rt:a0:a1 0x3c:rs:a0:a1 0x3c:rt:a0:a1 0x44:rs:a0:a1 0x44:rt:a1:v1 0x4c:rs:v1:a0 0x50:rt:a0:a1 0x54:rt:a0:a1 0x70:rs:v1:a0 0x94:rd:a1:v1 0x98:rs:v0:v1 0x98:rt:a1:v0 0x9c:rt:a1:v1 */
#include "types.h"

extern s32 pad_trig;
extern s32 name_ent_kanji_row;
extern s32 name_ent_yomi_no;
extern s32 name_ent_lower_col;
extern s32 name_ent_lower_row;

void name_ent_index_set(void)
{
    s32 old;
    s32 col;
    s32 row;
    s32 idx;

    old = name_ent_yomi_no;
    if (pad_trig & 0x20) {
        col = name_ent_lower_col;
        if (col != -1) {
            row = name_ent_lower_row;
            if (row != -1) {
                idx = row * 15 + col % 6 + col / 6 * 5;
                name_ent_yomi_no = idx;
                if (idx != old) {
                    name_ent_kanji_row = 0;
                }
            }
        }
    }
}
