/* Address: 0x8004FF8C */
/* Size: 164 bytes (41 instructions) */
/* Region: Game */
/* @SymbolSize: 164 */
/* @CompileFlags: -cckr */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x4:rt:a3:v0 0xc:rs:a3:v0 */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    u8 w;
    u8 h;
    u8 hit;
    u8 pad;
} Row;

typedef struct {
    u8 pad0[3984];
    Row row[16][2];
    u8 cnt[3];
    s8 sel[2];
    s8 chosen[2];
} Save;

extern Save screen_w;
extern s32 pad_data;

void menu_check_2(slot, arg1, arg2)
    s32 slot;
    s32 arg1;
    s32 arg2;
{
    s32 chosen;
    s32 i;
    Save *sv;

    sv = &screen_w;
    chosen = -1;
    sv->chosen[slot] = -1;
    for (i = 0; i < sv->cnt[slot]; i++) {
        if (i != sv->sel[slot]) {
            sv->row[i][slot].hit = 0;
        } else {
            sv->row[i][slot].hit = 10;
            chosen = i;
        }
    }
    if (!(pad_data & 4) && chosen != -1) {
        sv->chosen[slot] = chosen;
    }
}
