/* Function: title_move_show */
/* Address: 0x80062110 */
/* Size: 200 bytes (50 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x80062110 */
/* @SymbolSize: 200 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern u8 step_mode;
extern u32 step_counter;
extern s32 pad_trig;
extern u8 back_clear_g;
extern u8 back_clear_r;
extern u8 back_clear_b;
extern u8 screen_bri;
extern LIBGS_BG Bg[];

extern void sub_sub_step_up(void);
extern void back_clear_switch(s32 arg0);
extern void all_object_disp_off(void);
extern void sub_step_up(void);

void title_move_show(void) {
    u32 a0 = screen_bri;
    u8 *p797;
    u8 *p796;
    u8 *p798;

    if (a0 < 0x80) {
        (*(s8 *)&Bg[0].g) = a0;
        (*(s8 *)&Bg[0].b) = a0;
        (*(s8 *)&Bg[0].r) = a0;
        screen_bri = (u8)(a0 + 1);
    }

    if ((step_mode == 0) && (step_counter >= 0x259U)) {
        sub_sub_step_up();
    }

    if (pad_trig & 0x860) {
        p797 = &back_clear_g;
        p796 = &back_clear_r;
        p798 = &back_clear_b;
        *p796 = *p797 = *p798 = 255;
        back_clear_switch(1);
        all_object_disp_off();
        screen_bri = 0x80;
        sub_step_up();
    }
}
