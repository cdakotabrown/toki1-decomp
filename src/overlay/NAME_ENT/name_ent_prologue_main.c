/* @SymbolStart: 0x8014AE5C */
/* @RodataBase: 0x8014CBD0 */
/* @SymbolSize: 536 */
/* name_ent_prologue_main - NAME_ENT overlay, Address: 0x8014AE64, Size: 528 bytes (132 insns) */
#include "types.h"

extern void name_ent_prologue_start_main(void);
extern void name_ent_sub_sub_dispatch(void);
extern void name_ent_prologue_fall_obj_main(void);
extern void name_ent_start_gate(void);
extern void name_ent_load_gate(void);
extern void name_ent_prologue_intro_load_main(void);
extern void name_ent_prologue_serifu5_main(void);
extern void name_ent_prologue_serifu11_main(void);
extern void name_ent_prologue_serifu12_main(void);
extern void name_ent_prologue_serifu13_main(void);
extern void name_ent_maina(void);
extern void name_ent_mainb(void);
extern void name_ent_mainc(void);
extern void name_ent_maind(void);
extern void name_ent_maine(void);
extern void name_ent_mainf(void);
extern void name_ent_prologue_serifu32_main(void);
extern void name_ent_prologue_bg_load_main(void);
extern void name_ent_prologue_serifu38_main(void);
extern void name_ent_prologue_flash_main(void);
extern void name_ent_prologue_serifu42_main(void);
extern void name_ent_prologue_exit_main(void);

extern void basyo_show(void);
extern void update_anim_state(s32 a);
extern void message_window_show(void);
extern void goto_sub_step(s32 a);

extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern u32 pad_trig;
extern u8 now_sub_step;

void name_ent_prologue_main(void)
{
    switch (now_sub_step) {
    case 0:  name_ent_prologue_start_main(); break;
    case 1:  name_ent_sub_sub_dispatch(); break;
    case 2:  name_ent_prologue_fall_obj_main(); break;
    case 3:  name_ent_start_gate(); break;
    case 4:  name_ent_load_gate(); break;
    case 5:  name_ent_prologue_intro_load_main(); break;
    case 6:  name_ent_prologue_serifu5_main(); break;
    case 7:  name_ent_prologue_serifu11_main(); break;
    case 8:  name_ent_prologue_serifu12_main(); break;
    case 9:  name_ent_prologue_serifu13_main(); break;
    case 10: name_ent_maina(); break;
    case 11: name_ent_mainb(); break;
    case 12: name_ent_mainc(); break;
    case 13: name_ent_maind(); break;
    case 14: name_ent_maine(); break;
    case 15: name_ent_mainf(); break;
    case 16: name_ent_prologue_serifu32_main(); break;
    case 17: name_ent_prologue_bg_load_main(); break;
    case 18: name_ent_prologue_serifu38_main(); break;
    case 19: name_ent_prologue_flash_main(); break;
    case 20: name_ent_prologue_serifu42_main(); break;
    case 21: name_ent_prologue_exit_main(); break;
    default: break;
    }

    basyo_show();
    if (now_sub_step == 0) {
        update_anim_state(0);
    } else {
        update_anim_state(2);
    }
    message_window_show();
    if (now_sub_sub_step != 0 || now_sub_step != 0) {
        if (pad_trig & 0x800) {
            goto_sub_step(0x15);
        }
    }
}
