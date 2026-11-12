/* @SymbolSize: 356 */
/* @PostProc: patch_stack_complete --old 40 --new 72 */
/* @PostProc: remap_stack_slots --map '24:60,28:56,32:48,36:44' */
/* Address: 0x80140A64 */
/* Size: 356 bytes (89 instructions) */
#include "types.h"

extern s32 etc_flags_judge(void);
extern s32 etc_flags_count_check(void);
extern s32 etc_flags_count13_check(void);
extern void device_cmd_send(s32);

extern u8 date2_menu_select_tbl1;
extern u8 ev_flag07;
extern u8 ev_flag3b;
extern u8 ev_flag4d;
extern s32 etc_call_choice;
extern s32 etc_call_pass_cnt;
extern u8 mascot_flag2[];

void date2_menu_call_judge(void)
{
    s32 flagB;
    s32 flagA;
    s32 res2;
    s32 res1;
    s32 res3;
    s32 mode;
    s32 sel;

    flagB = 0;
    flagA = 0;
    res1 = etc_flags_judge();
    res2 = etc_flags_count_check();
    res3 = etc_flags_count13_check();

    if (ev_flag4d != 0 || ev_flag07 != 0 || ev_flag3b != 0) {
        flagA = 1;
    }
    if (mascot_flag2[11] >= 5U) {
        flagB = 1;
    }

    mode = etc_call_pass_cnt;
    if (mode == 0) {
        sel = res1;
        if (sel == 1) {
            if (date2_menu_select_tbl1 == 1U) {
                if (res2 == 1 || res3 == 1 || flagA == 1 || flagB == 1) {
                    goto doCall;
                }
            }
        }
        if (sel == 2) {
            if (date2_menu_select_tbl1 == 1U) {
doCall:
                device_cmd_send(1294);
                etc_call_choice = 1;
            }
        }
    } else if (mode == 1) {
        if (date2_menu_select_tbl1 == 1U) {
            device_cmd_send(1294);
            if (etc_call_choice == 1) {
                etc_call_choice = 3;
            } else {
                etc_call_choice = 2;
            }
        }
    }
}
