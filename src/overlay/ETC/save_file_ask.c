/* @SymbolStart: 0x80144644 */
/* @SymbolSize: 128 */
/* Address: 0x8014464C */
/* Size: 128 bytes (32 instructions) */
#include "types.h"

extern void sub_step_up(void);
extern void goto_sub_step(s32);
extern void text_sys_init(void);
extern void set_kanji_string(s32, s32, s32, void *, s32);
extern void k_disp_start(s32);
extern void card_step_init(void);

extern u8 save_file_ask_str0;

extern s32 pad_trig;

void save_file_ask(void)
{
    s32 v0 = pad_trig;

    if ((v0 & 0x20) != 0) {
        text_sys_init();
        set_kanji_string(-144, -64, 15, &save_file_ask_str0, 0);
        k_disp_start(1);
        card_step_init();
        sub_step_up();
        return;
    }
    if ((v0 & 0x40) == 0) {
        return;
    }
    goto_sub_step(2);
}
