/* @RodataBase: 0x8013CD64 */
/* @SymbolStart: 0x80139A50 */
/* @SymbolSize: 1036 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    s32 trg;
    s32 pad4;
    s32 held;
} PadState;

extern PadState pad_data;
extern u8  option_auto_flag;
extern u8  option_main_vab_wk;
extern u8  option_sub_vab_wk;
extern u16 option_xa_no;
extern u8  now_girl;
extern u8  now_icon;

void option_input_step()
{
    s32 k;
    s32 m;

    if (pad_data.held & 0x08000000) {
        option_auto_flag = 1 - option_auto_flag;
    }
    if (option_auto_flag != 0) {
        if (get_CD_State() & 2) {
            pad_data.held |= 0x20;
            pad_data.trg |= 4;
        }
    }
    if (pad_data.trg & 5) {
        if (pad_data.held & 0x8000) {
            now_girl++;
        }
        if (pad_data.held & 0x2000) {
            now_girl--;
        }
        if (now_girl == 255) {
            now_girl = 15;
        }
        if (now_girl >= 17) {
            now_girl = 0;
        }
    }
    if (option_main_vab_wk | option_sub_vab_wk) {
        option_vab_trans_dispatch();
    } else if (pad_data.held & 0x860) {
        switch (now_icon) {
        case 0:
            option_xa_call(0);
            break;
        case 1:
            goto_sub_sub_step(3);
            break;
        case 2:
            goto_sub_sub_step(4);
            break;
        case 3:
            option_port0_init();
            break;
        case 4:
            option_port1_init();
            break;
        case 5:
            if (pad_data.held & 0x800) {
                goto_sub_sub_step(5);
            } else {
                option_se_call();
            }
            break;
        case 6:
            option_seq_num_step();
            break;
        }
    }
    k = pad_data.trg & 0xF00000;
    m = pad_data.held & 0xF00000;
    if (pad_data.trg & 0x50000) {
        switch (k) {
        case 0x200000:
            option_xa_no += 1;
            break;
        case 0x400000:
            option_xa_no += 10;
            break;
        case 0x800000:
            option_xa_no += 100;
            break;
        case 0x100000:
            option_xa_no += 1000;
            break;
        }
    } else {
        switch (m) {
        case 0x200000:
            option_xa_no += 1;
            break;
        case 0x400000:
            option_xa_no += 10;
            break;
        case 0x800000:
            option_xa_no += 100;
            break;
        case 0x100000:
            option_xa_no += 1000;
            break;
        }
    }
    if (pad_data.trg & 0xA0000) {
        switch (k) {
        case 0x200000:
            if (option_xa_no - 1 >= 0) {
                option_xa_no -= 1;
            }
            break;
        case 0x400000:
            if (option_xa_no - 10 > 0) {
                option_xa_no -= 10;
            }
            break;
        case 0x800000:
            if (option_xa_no - 100 > 0) {
                option_xa_no -= 100;
            }
            break;
        case 0x100000:
            if (option_xa_no - 1000 > 0) {
                option_xa_no -= 1000;
            }
            break;
        }
    }
    if (option_xa_no >= 2900) {
        option_xa_no = 0;
    }
    option_debug_show();
}
