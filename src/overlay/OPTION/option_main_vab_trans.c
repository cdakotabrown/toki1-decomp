/* @SymbolStart: 0x8013A1DC */
/* @SymbolSize: 328 */
/* Address: 0x8013A1E4 (window 0x8013A1DC..0x8013A324, 328 bytes = 82 insns) */
#include "types.h"

extern void device_load_start();
extern s32 device_load_poll();
extern void strcpy();

extern u8 option_main_vab_no;
extern u8 option_main_vab_wk;
extern u8 option_se_bank_no;
extern s32 option_se_body_tbl[];
extern s32 option_se_head_tbl[];
extern s32 option_main_vab_seq_tbl[];
extern s32 option_main_vab_body_tbl[];
extern s32 option_main_vab_head_tbl[];
extern u8 option_main_vab_name_buf[];
extern u8 option_se_name_buf[];
extern u8 option_main_vab_name_tbl[];
extern u8 option_se_name_tbl[];

void option_main_vab_trans(void) {
    s32 idx;
    s32 idx2;
    u32 step;

    step = option_main_vab_wk;
    switch (step) {
    case 1:
        idx = (u32)option_se_bank_no % 58;
        idx2 = (u32)option_main_vab_no % 63;
        device_load_start(0, option_se_head_tbl[idx], option_se_body_tbl[idx], option_main_vab_head_tbl[idx2],
                      option_main_vab_body_tbl[idx2], option_main_vab_seq_tbl[idx2]);
        step = option_main_vab_wk;
        option_main_vab_wk = step + 1;
        break;
    case 2:
        if (device_load_poll() == 1) {
            idx = (u32)option_main_vab_no % 63;
            option_main_vab_wk = 0;
            strcpy(option_main_vab_name_buf, &option_main_vab_name_tbl[idx * 16]);
            idx2 = (u32)option_se_bank_no % 58;
            strcpy(option_se_name_buf, &option_se_name_tbl[idx2 * 16]);
        }
        break;
    }
}
