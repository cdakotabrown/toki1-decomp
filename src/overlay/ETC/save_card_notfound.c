/* @SymbolStart: 0x80145168 */
/* @SymbolSize: 324 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-8 IDO52_AS1_PEEPREG_KEEP=0 */
/* save_card_notfound - ETC overlay, Address: 0x80145170, Size: 316 bytes (79 insns) */
#include "types.h"

extern void goto_sub_sub_step();
extern void device_cmd_send();
extern void set_kanji_string();
extern void k_sub_reset();
extern void k_sub_disp_start();
extern void card_step_init();
extern void card_state_clear();
extern u8 step_mode;
extern s32 pad_trig;
extern u8 save_card_notfound_str0[];
extern u8 save_card_notfound_str1[];
extern u8 save_card_notfound_str2[];

s32 save_card_notfound(void)
{
    u32 v1 = step_mode;

    switch (v1) {
    case 0:
        card_state_clear();
        device_cmd_send(1282);
        k_sub_reset();
        set_kanji_string(-128, 50, 1, save_card_notfound_str0, 0);
        k_sub_disp_start(1);
        step_mode = step_mode + 1;
        break;
    case 1:
        if ((pad_trig & 0x0860) != 0) {
            step_mode = step_mode + 1;
        }
        break;
    case 2:
        k_sub_reset();
        set_kanji_string(-128, 50, 1, save_card_notfound_str1, 0);
        set_kanji_string(-128, 66, 1, save_card_notfound_str2, 0);
        k_sub_disp_start(2);
        step_mode = step_mode + 1;
        break;
    case 3:
        if ((pad_trig & 0x0860) != 0) {
            card_step_init();
            goto_sub_sub_step(0);
        }
        break;
    }
}
