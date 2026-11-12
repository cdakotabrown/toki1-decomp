/* Likely Symbol: Yubi (from o.bin, high match) */
/* Address: 0x800865F4 */
/* Size: 80 bytes (20 instructions) */
/* Compiler: IDO 5.2 */
/* Region: IDO (0x80086000-0x800BFFFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

typedef struct {
    u8 pad[38];
    s16 field_26;
    u8 pad2[2];
    s16 field_2A;
    u8 pad3[24];
} Struct68;

extern Struct68 class_w[];
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern void step_directional_scroll_once(void);

void Yubi(s32 index) {
    step_directional_scroll_once();
    class_w[index].field_26 = cursor_cw_x;
    class_w[index].field_2A = cursor_cw_y;
}
