/* Address: 0x8004EBB0 (the .sym recorded the prologue, 0x8004EBB8) */
/* Size: 252 bytes (63 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x8004EBB0 */
/* @SymbolSize: 252 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: strip_trailing_instructions --count 1 --nop-only */
/* @PostProc: swap_registers t1 t9 --only 0xC4,0xC8,0xD0,0xD4 --bidirectional */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

typedef struct {
    s16 field_00;
    s16 field_02;
    u8 field_04;
    u8 field_05;
    u8 field_06;
    u8 field_07;
} TextEntry;

extern s16 disp_string_state;
extern TextEntry disp_string_tbl0[];

extern s32 load_string(u8 *str);

s16 set_kanji_string(s16 arg0, s16 arg1, u8 arg2, u8 *arg3, s32 arg4) {
    TextEntry *v1;
    s32 result;

    if (disp_string_state >= 52) {
        return -1;
    }

    if (arg3 == 0) {
        disp_string_tbl0[disp_string_state].field_07 = 0;
    } else if (arg3[0] == 0 || arg3[1] == 0) {
        disp_string_tbl0[disp_string_state].field_07 = 0;
    } else {
        v1 = &disp_string_tbl0[disp_string_state];
        v1->field_02 = arg1;
        v1->field_04 = arg2;
        v1->field_00 = arg0;
        result = load_string(arg3);
        v1 = &disp_string_tbl0[disp_string_state];
        v1->field_05 = (u8)result;
        v1->field_07 = 1;
        v1->field_06 = (u8)arg4;
    }

    disp_string_state = disp_string_state + 1;
    return disp_string_state;
}
