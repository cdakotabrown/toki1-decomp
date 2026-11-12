/* Function: vload_font */
/* Address: 0x8004F7A0 */
/* Size: 396 bytes (99 instructions) over the corrected window */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x8004F78C */
/* @SymbolSize: 396 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* gone (2026-08-23) in favour of `@CompileFlags: -Wc,-pic1`, and the */
#include "types.h"

extern s16 check_end_k_tbl0;
extern s32 font_buf[];

extern void melt_font(s32, s32 *);
extern void load_image_rect(s32, s32, s32, s32, s32 *);
extern void DrawSync(s32);

void vload_font(s32 idx) {
    u16 x;
    u16 y;

    melt_font(idx, &font_buf[(check_end_k_tbl0 & 0xF) * 24]);

    if (check_end_k_tbl0 < 128) {
        x = (u16)(((check_end_k_tbl0 & 7) << 2) + 800);
        y = (u16)(((check_end_k_tbl0 & 0x78) << 1) + 256);
        check_end_k_tbl0 = (s16)check_end_k_tbl0;
        load_image_rect(x, y, 4, 12, &font_buf[(check_end_k_tbl0 & 0xF) * 24]);
    } else if (check_end_k_tbl0 < 256) {
        x = (u16)(((check_end_k_tbl0 & 7) << 2) + 864);
        y = (u16)(((check_end_k_tbl0 & 0x78) << 1) + 256);
        check_end_k_tbl0 = (s16)check_end_k_tbl0;
        load_image_rect(x, y, 4, 12, &font_buf[(check_end_k_tbl0 & 0xF) * 24]);
    } else {
        x = (u16)(((check_end_k_tbl0 & 7) << 2) + 928);
        y = (u16)(((check_end_k_tbl0 & 0x78) << 1) + 256);
        check_end_k_tbl0 = (s16)check_end_k_tbl0;
        load_image_rect(x, y, 4, 12, &font_buf[(check_end_k_tbl0 & 0xF) * 24]);
    }

    DrawSync(0);
    check_end_k_tbl0 = check_end_k_tbl0 + 1;
}
