/* @SymbolStart: 0x8014A534 */
/* @RodataBase: 0x8014CB64 */
/* @SymbolSize: 540 */
/* Address: 0x8014A53C, Size: 532 bytes (133 insns) */
#include "types.h"

extern u8 step_mode;
extern u8 name_ent_serifu_no;
extern s32 dec_bg_handle;
extern u32 Bg;

extern void name_ent_serifu_set();
extern void name_ent_text_scroll();
extern void name_ent_kuchi_ani6();
extern void device_port_init();
extern void value_cache_find_or_load();
extern s32 get_CD_State2();
extern s32 dec_bg_init();
extern void indexed_byte_lsb_set();
extern void dec_bg_reset();
extern void class_clear_f();
extern void sub_step_up();
extern void dec_bg_show();

void name_ent_prologue_serifu32_body(void)
{
    s32 sel;
    sel = step_mode;
    switch (sel) {
        case 0:
            name_ent_serifu_no = 32;
            name_ent_serifu_set();
            step_mode = step_mode + 1;
            break;
        case 1:
            name_ent_text_scroll();
            name_ent_kuchi_ani6();
            break;
        case 2:
            name_ent_serifu_no = 33;
            name_ent_serifu_set();
            step_mode = step_mode + 1;
            break;
        case 3:
            name_ent_text_scroll();
            break;
        case 4:
            device_port_init(0, 1);
            name_ent_serifu_no = 34;
            name_ent_serifu_set();
            step_mode = step_mode + 1;
            break;
        case 5:
            name_ent_text_scroll();
            name_ent_kuchi_ani6();
            break;
        case 6:
            name_ent_serifu_no = 35;
            name_ent_serifu_set();
            step_mode = step_mode + 1;
            break;
        case 7:
            name_ent_text_scroll();
            break;
        case 8:
            name_ent_serifu_no = 36;
            name_ent_serifu_set();
            step_mode = step_mode + 1;
            break;
        case 9:
            name_ent_text_scroll();
            name_ent_kuchi_ani6();
            break;
        case 10:
            value_cache_find_or_load(18794, 1);
            step_mode = step_mode + 1;
            break;
        case 11:
            if (get_CD_State2() & 1) {
                indexed_byte_lsb_set(1, dec_bg_init(1));
                if (dec_bg_handle == -1) {
                    dec_bg_reset();
                    value_cache_find_or_load(18794, 1);
                } else {
                    class_clear_f();
                    Bg = 0x80000000;
                    sub_step_up();
                }
            }
            break;
    }

    dec_bg_show(1);
}
