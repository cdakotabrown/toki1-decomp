/* Address: 0x80149AE8, Size: 164 bytes (41 insns) */
/* @SymbolSize: 164 */
/* @PostProc: patch_stack_complete --old 40 --new 48 */
/* @PostProc: remap_stack_slots --map '36:40,37:41' */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x3c */
/* @PostProc: patch_insn_reg_field 0x1c:rt:v1:v0 0x24:rs:v1:v0 0x24:rt:v1:v0 0x30:rd:v0:t1 0x30:rt:v1:v0 0x3c:rs:v0:t1 0x3c:rt:v0:t2 0x40:rs:v1:v0 0x44:rt:v0:t2 0x48:rt:t1:t4 0x4c:rd:t2:t5 0x4c:rt:v0:t2 0x50:rs:t1:t4 0x50:rt:t1:t4 0x54:rd:t3:t9 0x54:rt:t2:t5 0x58:rd:t4:t8 0x58:rt:t3:t9 0x5c:rd:t5:t6 0x5c:rs:t1:t4 0x5c:rt:t4:t8 0x60:rs:t5:t6 0x60:rt:v0:t7 0x64:rt:t9:t3 0x68:rs:v0:t7 0x68:rt:t8:t0 0x6c:rt:t7:t5 0x70:rs:t8:t0 0x70:rt:t9:t3 0x74:rs:t7:t5 0x74:rt:t7:t5 0x78:rt:t6:t2 0x7c:rs:t6:t2 0x7c:rt:t6:t2 0x80:rd:t0:t9 0x80:rt:t7:t5 0x84:rd:t2:t4 0x84:rs:t6:t2 0x84:rt:t0:t9 0x88:rs:t2:t4 0x88:rt:v0:t8 0x8c:rt:t3:t1 0x90:rs:v0:t8 0x90:rt:t1:t6 0x98:rs:t1:t6 0x98:rt:t3:t1 */
#include "types.h"

typedef struct { u8 b0, b1, b2; } Rec3;

extern s32 sub_sub_step_up();
extern s32 serifu_no;
extern Rec3 date_serifu_row_set_tbl;
extern s16 date_now_serifu2;
extern s32 **date_ss_serifu2;

s32 date_serifu_row_set(void) {
    Rec3 rec;
    s32 v0;
    s32 id;

    v0 = serifu_no;
    rec = date_serifu_row_set_tbl;
    id = v0 * 2 + 13;
    date_now_serifu2 = (s16)id;
    if (v0 == 0) {
        s32 *p;

        p = date_ss_serifu2[(s16)id + 1];
        *(u8*)*p = rec.b0;

        p = date_ss_serifu2[date_now_serifu2 + 1];
        *(u8*)(*p + 1) = rec.b1;
    }
    return sub_sub_step_up();
}
