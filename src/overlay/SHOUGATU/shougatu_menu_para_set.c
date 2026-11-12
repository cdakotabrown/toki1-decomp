/* Address: 0x80140228 (cross-prologue head absorbed from 0x80140214) */
/* Size: 152 bytes (38 instructions incl. hoisted head) */
/* @SymbolStart: 0x80140214 */
/* @SymbolSize: 152 */
/* @PostProc: patch_insn_reg_field 0x18:rt:v0:t7 0x1c:rd:v0:t7 0x1c:rs:v0:t7 0x20:rs:v0:t7 0x20:rt:v0:t7 0x2c:rs:v0:t7 0x4c:rt:t7:t8 0x50:rs:t7:t8 0x50:rt:t7:t8 0x54:rt:v1:t9 0x58:rd:v1:t9 0x58:rs:v1:t9 0x58:rt:t7:t8 0x5c:rs:v1:t9 0x5c:rt:v1:t9 0x60:rt:t9:t1 0x64:rd:t8:t0 0x64:rt:v1:t9 0x68:rd:t8:t0 0x68:rs:t8:t0 0x68:rt:v1:t9 0x6c:rd:t8:t0 0x6c:rt:t8:t0 0x70:rs:t9:t1 0x70:rt:t9:t1 0x74:rs:t8:t0 0x74:rt:t9:t1 0x78:rt:t0:t2 0x80:rs:t0:t2 0x80:rt:t1:t3 0x84:rt:t1:t3 */
#include "types.h"

extern void device_cmd_send(s32);
extern void param_clamp(void);
extern void sub_sub_step_up(void);

extern u8 shougatu_menu_slot;
extern u8 shougatu_menu_girl_tbl;
extern s16 screen_w;

void shougatu_menu_para_set(void)
{
    s32 sel;
    s32 idx;

    sel = (&shougatu_menu_girl_tbl)[shougatu_menu_slot];
    if (sel == 11) {
        device_cmd_send(512);
    } else {
        device_cmd_send(513);
        idx = (&shougatu_menu_girl_tbl)[shougatu_menu_slot];
        *(s16 *)((u8 *)&screen_w + idx * 56 + 450) += 1;
    }
    param_clamp();
    sub_sub_step_up();
}
