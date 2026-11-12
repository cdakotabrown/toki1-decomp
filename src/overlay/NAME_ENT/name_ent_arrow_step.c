/* @CompileFlags: -cckr */
/* @SymbolStart: 0x80135700 */
/* @SymbolSize: 420 */
/* Address: 0x80135700 (absorbed head) / 0x80135708 (prologue) */
/* Size: 412 bytes (103 insns), 420 bytes / 105 words including the head. */
#include "types.h"

extern s32 name_ent_tab_no;
extern volatile s16 cursor_cw_x;
extern volatile s16 cursor_cw_y;
extern volatile s16 cw121_frame;
extern volatile s16 cw122_frame;
extern volatile u8 cw121_disp;
extern volatile u8 cw122_disp;
extern volatile u8 cw123_disp;
extern volatile u8 cw124_disp;
extern u32 pad_data;
extern u32 pad_trig;
extern s32 name_ent_kanji_row;

extern void name_ent_desc_disp();

name_ent_arrow_step()
{
    s32 state;

    state = name_ent_tab_no;

    if (state == 2 || state == 3) {
        state = cursor_cw_x;
        cw121_frame = 25;
        cw122_frame = 27;
        cw121_disp = 128;
        cw122_disp = 128;
        cw123_disp = 0;
        cw124_disp = 0;

        if (state >= 137 && state < 152) {
            state = cursor_cw_y;
            if (state >= -47 && state < -32) {
                cw121_frame = 26;
                if (pad_data & 0x0020) {
                    cw123_disp = 128;
                }
                if (pad_trig & 0x0020) {
                    if (name_ent_kanji_row > 0) {
                        name_ent_kanji_row--;
                    }
                    name_ent_desc_disp(0);
                }
                return;
            }
            if (state >= 17 && state < 32) {
                cw122_frame = 28;
                if (pad_data & 0x0020) {
                    cw124_disp = 128;
                }
                if (pad_trig & 0x0020) {
                    name_ent_kanji_row++;
                    name_ent_desc_disp(0);
                }
            }
        }
    } else {
        cw121_disp = 0;
        cw122_disp = 0;
        cw123_disp = 0;
        cw124_disp = 0;
    }
}
