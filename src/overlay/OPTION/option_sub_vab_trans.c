/* @SymbolStart: 0x8013A0BC */
/* option_sub_vab_trans - OPTION overlay, Address: 0x8013A0C4, Size: 280 bytes (70 insns) */
/* @SymbolSize: 288 */
/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: patch_insn_reg_field 0x90:rt:t0:v0 0x94:rs:t0:v0 0x94:rt:t0:v0 0x9c:rs:t0:v0 0x9c:rt:t1:t0 0xa4:rt:t1:t0 0xb8:rt:t2:t1 0xbc:rs:t2:t1 0xbc:rt:t2:t1 0xc4:rs:t2:t1 0xc8:rt:t5:t4 0xcc:rs:t5:t4 0xcc:rt:t5:t4 0xe0:rd:t3:t2 0xe4:rd:t4:t3 0xe4:rt:t3:t2 0xec:rs:t4:t3 0xec:rt:t5:t4 0xf0:rt:t6:t5 0xf4:rs:t6:t5 0xf4:rt:t6:t5 0xfc:rs:t6:t5 0x100:rt:t9:t8 0x104:rs:t9:t8 0x104:rt:t9:t8 0x110:rd:t7:t6 0x114:rd:t8:t7 0x114:rt:t7:t6 0x11c:rs:t8:t7 0x11c:rt:t9:t8 */
#include "types.h"

extern u8 option_sub_vab_wk;

extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);
extern void strcpy(s32, s32);

extern u8 option_sub_vab_no;
extern u8 option_sub_vab_wk;
extern u8 option_se_bank_no;
extern s32 option_sub_vab_seq_tbl[];
extern s32 option_sub_vab_body_tbl[];
extern s32 option_sub_vab_head_tbl[];
extern u8 option_sub_vab_name_tbl[];
extern u8 option_se_name_tbl[];
extern u8 option_sub_vab_name_buf;
extern u8 option_se_name_buf;

void
option_sub_vab_trans(void)
{
    s32 sel;
    s32 idx;
    s32 idx2;

    sel = option_sub_vab_wk;

    switch (sel) {
    case 1:
        idx = (u32)option_sub_vab_no % 63;
        device_load_start(1, 0xBFA6, 0xBF87, option_sub_vab_head_tbl[idx], option_sub_vab_body_tbl[idx],
                      option_sub_vab_seq_tbl[idx]);
        option_sub_vab_wk = option_sub_vab_wk + 1;
        break;
    case 2:
        if (device_load_poll() != 1) {
            break;
        }
        idx = (u32)option_sub_vab_no % 63;
        option_sub_vab_wk = 0;
        strcpy((s32)&option_sub_vab_name_buf, (s32)(option_sub_vab_name_tbl + idx * 16));
        idx2 = (u32)option_se_bank_no % 58;
        strcpy((s32)&option_se_name_buf, (s32)(option_se_name_tbl + idx2 * 16));
        break;
    }
}
