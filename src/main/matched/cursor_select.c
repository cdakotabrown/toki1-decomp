/* Function: cursor_select */
/* Address: 0x800625F8 */
/* Size: 320 bytes (80 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers_auto --swaps a0:v1 --unidirectional --exclude-offsets 0x9c,0xa4 */
#include "types.h"

extern u8 pad_type;
extern s32 work_fuku;
extern s16 cursor_cw_y;
extern s16 cursor_cw_x;
extern s32 pad_trig;
extern s32 step_counter;
extern s8 now_icon;
extern s16 cw97_frame;
extern s16 cw98_frame;

extern void menu_check(s32, s16, s16);
extern void menu_cursor_move(s32);

void cursor_select(void) {
    s32 v1;
    s32 val;

    if (pad_type != 0) {
        v1 = work_fuku;
        cursor_cw_y = (s16)((v1 << 4) + 70);
        cursor_cw_x = 64;

        switch (pad_trig & 0x5000) {
            case 0x1000:
                step_counter = 0;
                v1 = 0;
                break;
            case 0x4000:
                step_counter = 0;
                v1 = 1;
                break;
        }
        work_fuku = v1;
    } else {
        menu_check(0, cursor_cw_x, cursor_cw_y);
        menu_cursor_move(0);

        val = now_icon;
        switch (val) {
            case 0:
                step_counter = 0;
                work_fuku = 0;
                break;
            case 1:
                v1 = 1;
                step_counter = 0;
                work_fuku = v1;
                break;
        }
    }

    v1 = work_fuku;
    if (v1 != 0) {
        cw97_frame = 2;
        cw98_frame = 3;
    } else {
        cw97_frame = 1;
        cw98_frame = 4;
    }
}
