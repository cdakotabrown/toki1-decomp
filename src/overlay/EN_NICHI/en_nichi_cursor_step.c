/* @SymbolSize: 172 */
/* en_nichi_cursor_step - EN_NICHI overlay, Address: 0x80132FB8, Size: 200 bytes (50 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern void en_nichi_cursor_move(void);
extern void en_nichi_marker_add(s16 a0, s16 a1);
extern void en_nichi_cmd_send_once(s32 a0);

extern s32 pad_data;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern CLASS_W class_w[];

void en_nichi_cursor_step(void)
{
    en_nichi_cursor_move();

    if (pad_data & 0x0020) {
        if (class_w[152].prio != 7) {
            en_nichi_marker_add(cursor_cw_x, cursor_cw_y);
            en_nichi_cmd_send_once(1281);
        }
        class_w[152].prio = 7;
        return;
    }
    if (class_w[152].prio != 3) {
        en_nichi_marker_add(cursor_cw_x, cursor_cw_y);
        en_nichi_cmd_send_once(1282);
    }
    class_w[152].prio = 3;
}
