/* Address: 0x8013A354, Size: 336 bytes (84 instructions) */
#include "types.h"

extern void _debug_number_show();
extern void _debug_string_show();

extern u16 option_xa_no;
extern u8 now_girl;
extern u8 option_main_vab_wk;
extern u8 option_sub_vab_wk;
extern u8 option_main_vab_no;
extern u8 option_main_vab_name_buf[];
extern u8 option_sub_vab_no;
extern u8 option_sub_vab_name_buf[];
extern s8 option_debug_val;
extern s32 option_se_no;
extern u8 option_se_name_buf[];
extern u8 option_seq_no;

void option_debug_show(void) {
    s32 a0;
    s32 val;

    _debug_number_show((s32)option_xa_no, 80, -60, 6);

    a0 = now_girl;
    val = a0 & 0xF;
    _debug_number_show(val, -80, -60, 6);

    _debug_number_show((s32)option_main_vab_wk, 80, -40, 6);
    _debug_number_show((s32)option_sub_vab_wk, 80, -20, 6);
    _debug_number_show((s32)option_main_vab_no, 80, 0, 6);

    _debug_string_show((s32)option_main_vab_name_buf, -150, 8, 6);
    _debug_number_show((s32)option_sub_vab_no, 80, 20, 6);
    _debug_string_show((s32)option_sub_vab_name_buf, -150, 20, 6);

    val = option_debug_val;
    _debug_number_show(val, -80, 10 - val * 10, 6);

    _debug_number_show(option_se_no, 80, 40, 6);
    _debug_string_show((s32)option_se_name_buf, -150, 32, 6);
    _debug_number_show((s32)option_seq_no, 80, 60, 6);
}
