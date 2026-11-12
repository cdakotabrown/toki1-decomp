/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: patch_insn_reg_field 0x44:rt:v1:v0 0x48:rt:v0:a0 0x4c:rs:v1:v0 0x50:rs:v0:a0 0x50:rt:v0:a0 0x54:rt:a1:a2 0x58:rs:a1:a2 0x58:rt:a1:a2 0x5c:rt:a2:a1 0x60:rs:v0:a0 0x60:rt:a0:v1 0x68:rs:a0:v1 0x6c:rs:a0:v1 0x74:rs:v1:v0 0x80:rs:v0:a0 0x90:rs:v0:a0 0x94:rs:v0:a0 0x94:rt:a2:a1 0x98:rs:v0:a0 0x9c:rs:v0:a0 0x9c:rt:a0:v1 0xa4:rs:a0:v1 0xa8:rs:a0:v1 0xb0:rs:v1:v0 0xbc:rs:v0:a0 0xcc:rs:v0:a0 0xd0:rs:v0:a0 0xd0:rt:a2:a1 0xd4:rs:v0:a0 0xd8:rs:v0:a0 0xd8:rt:v0:a0 0xdc:rs:v0:a0 0xdc:rt:a1:a2 0xe8:rt:v1:v0 */
/* @SymbolSize: 228 */
/* en_nichi_konami_command_set - EN_NICHI overlay, Address: 0x80136BA8, Size: 228 bytes (57 insns) */
#include "types.h"

extern u8 date2_menu_select_tbl0;
extern u8 date2_menu_select_tbl1;
extern s32 en_nichi_konami_cnt;
extern s32 en_nichi_obj_wk[];
extern s32 en_nichi_kind_attr_tbl[];

void en_nichi_konami_command_set(void)
{
    s32 cnt;
    s32 *p;
    s32 v;

    if (date2_menu_select_tbl1 != 0) {
        date2_menu_select_tbl0 = 0;
        date2_menu_select_tbl1 = 0;
        en_nichi_konami_cnt = en_nichi_konami_cnt + 1;
    }
    cnt = en_nichi_konami_cnt;
    if (cnt <= 0) {
        return;
    }
    p = en_nichi_obj_wk;
    do {
        v = p[0];
        if (v > 0 && v < 4) {
            if (cnt < 3) {
                p[0] = p[1] + 1;
            } else {
                p[0] = 4;
                p[1] = 0;
            }
        }
        v = p[12];
        if (v > 0 && v < 4) {
            if (cnt < 3) {
                p[12] = p[13] + 1;
            } else {
                p[12] = 4;
                p[13] = 0;
            }
        }
        p += 24;
    } while (p != en_nichi_kind_attr_tbl);
    en_nichi_konami_cnt = cnt;
}
