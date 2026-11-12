/* Address: 0x800674F0 */
/* Size: 128 bytes (32 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @SymbolStart: 0x800674E8 */
/* @SymbolSize: 128 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: swap_instructions 0x30 0x34 */
#include "types.h"

extern void load_image_rect(s32, s32, s32, s32, s32);
extern u8 screen_w[];
extern s32 cursor_work_bak;
extern s32 back_scroll_init_tbl0;

void back_scroll_init(void) {
    u8 *p;
    u8 *end;

    load_image_rect(992, 384, 32, 128, back_scroll_init_tbl0);
    p = screen_w;
    end = screen_w + 0x200;
    do {
        p += 16;
        *(s32 *)(p + 0x162C) = 0;
        *(s32 *)(p + 0x1630) = 0;
        *(s32 *)(p + 0x1634) = 0;
        *(s32 *)(p + 0x1638) = 0;
    } while (p != end);
    load_image_rect(992, 448, 4, 16, (s32)&cursor_work_bak);
}
