/* @SymbolStart: 0x8013614c */
/* @SymbolSize: 172 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x18:rt:v0:v1 0x1c:rd:v0:v1 0x1c:rs:v0:v1 0x20:rs:v0:v1 0x20:rt:a0:v1 0x28:rs:a0:v1 0x30:rt:a1:t7 0x34:rs:a1:t7 0x34:rt:a1:t7 0x3c:rs:a1:t7 0x40:rd:t7:t8 0x44:rd:t7:t8 0x44:rs:t7:t8 0x44:rt:a1:t7 0x48:rt:t8:t9 0x4c:rs:t8:t9 0x4c:rt:t8:t9 0x50:rd:t7:t8 0x50:rt:a1:t8 0x54:rd:v1:v0 0x54:rs:t7:t8 0x54:rt:t8:t9 0x58:rs:v1:v0 0x58:rt:v0:t0 0x60:rd:t9:t2 0x60:rt:v0:t0 0x64:rs:t9:t2 0x6c:rs:v1:v0 0x6c:rt:v0:t3 0x74:rs:v0:t3 0x74:rt:t0:t4 0x7c:rs:v1:v0 0x7c:rt:t0:t4 0x84:rs:a0:v1 0x88:rt:t1:t5 0x8c:rs:t1:t5 0x8c:rt:t1:t5 0x94:rs:t1:t5 0x94:rt:t2:t6 0x98:rt:t2:t6 */
/* kangei_choice_para_set - KANGEI overlay, Address: 0x80136160, Size: 152 bytes (38 insns) */
#include "types.h"

extern void param_clamp(void);
extern void sub_sub_step_up(void);

extern u8 kangei_choice_no;
extern u8 kangei_choice_tbl;

extern u8 girl_no;

extern u8 screen_w[];

extern s16 para_str;

void
kangei_choice_para_set(void)
{
    u32 status;
    s16 val;
    u8 charIdx;
    u8 idx;
    u8 sel;

    idx = kangei_choice_no;
    sel = *(&kangei_choice_tbl + (s32)idx);

    if (sel == 0) {

        charIdx = girl_no;
        if (charIdx != 0xFF) {

            status = *(u32 *)&screen_w[(u32)charIdx * 56 + 456];

            if ((s32)(status << 30) < 0) {

                val = *(s16 *)&screen_w[(u32)charIdx * 56 + 446];
                *(s16 *)&screen_w[(u32)charIdx * 56 + 446] =
                    (s16)(val - 10);
                goto tail;
            }
        }
    }

    if (sel == 1) {

        val = para_str;
        para_str = (s16)(val + 10);
    }

tail:
    param_clamp();
    sub_sub_step_up();
}
