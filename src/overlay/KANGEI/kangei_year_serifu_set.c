/* @SymbolStart: 0x8013852C */
/* @SymbolSize: 180 */
/* @PostProc: swap_instructions --offset1 0x9c --offset2 0xa0 */
/* @PostProc: patch_insn_reg_field 0x28:rt:v0:v1 0x2c:rs:v0:v1 0x2c:rt:v0:v1 0x34:rs:v0:v1 0x34:rt:v0:v1 0x38:rs:v0:v1 0x40:rd:v0:v1 0x44:rt:v0:v1 0x48:rs:v0:v1 0x48:rt:v0:v1 0x50:rs:v0:v1 0x50:rt:v0:v1 0x54:rs:v0:v1 0x58:rd:v0:v1 0x70:rt:t2:v0 0x78:rs:t2:v0 0x78:rt:t2:v0 0x90:rd:t3:t2 0x90:rt:t2:v0 0x94:rd:t3:t4 0x94:rs:t3:t2 0x94:rt:t2:v0 0x9c:rd:t4:t6 0x9c:rs:t3:t4 0x9c:rt:v0:v1 0xa4:rt:t4:t6 0xb0:rt:v0:v1 */
/* kangei_year_serifu_set - KANGEI overlay, Address: 0x80138534, Size: 172 bytes (43 insns) */
#include "types.h"

extern void sub_sub_step_up(void);

extern u8 now_year;
extern u8 now_girl;
extern s32 kangei_bank2_tbl_0_11;
extern s32 kangei_bank2_tbl_1_11;
extern s32 kangei_bank2_tbl_2_11;
extern s32 kangei_ss_s;
extern s32 kangei_ss_s_kind;
extern s32 kangei_sss_so;
extern s16 kangei_now_s;
extern u8 kangei_year_no;
extern u8 now_month;

void kangei_year_serifu_set(void)
{
    s32 t6_in = now_month;

    u8 col;
    u8 cur_char;

    if ((u32)t6_in >= 4U) {
        col = (u8)(now_year - 95);
    } else {
        col = (u8)(now_year - 96);
    }

    kangei_ss_s = kangei_bank2_tbl_0_11;
    kangei_ss_s_kind = kangei_bank2_tbl_1_11;
    kangei_sss_so = kangei_bank2_tbl_2_11;

    cur_char = now_girl;
    kangei_now_s = (s16)((u32)cur_char * 3 + (u32)col);
    kangei_year_no = col;
    sub_sub_step_up();
}
