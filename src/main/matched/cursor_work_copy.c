/* Likely Symbol: menu_step_main2 (from o.bin, offset +180 bytes from 0x8005C690) */
/* Address: 0x8005C744 */
/* Size: 240 bytes (60 instructions) */
/* Compiler: IDO 5.2 -O2 -G0 -mips1 */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "common.h"

extern void *memcpy(void *dst, void *src, s32 size);

extern u8 cursor_work_bak[64];
extern u8 icon_xy[64];

extern s32 cursor_bak_x;
extern s32 cursor_bak_y;
extern s32 cursor_bak_no;
extern s32 cursor_bak_hi;
extern s32 cursor_bak_cw;

extern s16 mascot_x;
extern s16 mascot_y;
extern s16 cursor_work_copy_tbl0;
extern u8 mascot_hi;
extern u8 mascot_cw;

void cursor_work_copy(s32 arg0) {
    if (arg0 != 0) {

        memcpy(icon_xy, cursor_work_bak, 64);
        mascot_x = (s16)cursor_bak_x;
        mascot_y = (s16)cursor_bak_y;
        cursor_work_copy_tbl0 = (s16)(u8)cursor_bak_no;
        mascot_hi = (u8)cursor_bak_hi;
        mascot_cw = (u8)cursor_bak_cw;
    } else {

        memcpy(cursor_work_bak, icon_xy, 64);
        cursor_bak_x = (s32)mascot_x;
        cursor_bak_y = (s32)mascot_y;
        cursor_bak_no = (s32)cursor_work_copy_tbl0;
        cursor_bak_hi = (s32)(u8)mascot_hi;
        cursor_bak_cw = (s32)(u8)mascot_cw;
    }
}
