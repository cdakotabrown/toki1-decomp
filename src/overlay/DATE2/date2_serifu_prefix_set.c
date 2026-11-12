/* date2_serifu_prefix_set -- DATE2 overlay, Address: 0x801378A4, Size: 164 bytes (41 insns) */
/* @PostProc: patch_stack_complete --old 56 --new 48 */
/* @PostProc: remap_stack_slots --from 52 --to 40 --from 53 --to 41 */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x3c */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x4c */
/* @PostProc: patch_insn_reg_field 0x1c:rt:v1:v0 0x24:rs:v1:v0 0x24:rt:v1:v0 0x30:rd:v0:t1 0x30:rt:v1:v0 0x3c:rs:v0:t1 0x3c:rt:v0:t2 0x40:rs:v1:v0 0x44:rt:v0:t2 0x4c:rd:t1:t5 0x4c:rt:v0:t2 0x54:rd:t2:t9 0x54:rt:t1:t5 0x58:rd:t3:t8 0x58:rt:t2:t9 0x5c:rd:t5:t6 0x5c:rs:t3:t4 0x5c:rt:t4:t8 0x60:rs:t5:t6 0x60:rt:v0:t7 0x64:rt:t9:t3 0x68:rs:v0:t7 0x68:rt:v1:t0 0x6c:rt:t8:t5 0x70:rs:v1:t0 0x70:rt:t9:t3 0x74:rs:t8:t5 0x74:rt:t8:t5 0x78:rt:t7:t2 0x7c:rs:t7:t2 0x7c:rt:t7:t2 0x80:rd:t6:t9 0x80:rt:t8:t5 0x84:rd:t0:t4 0x84:rs:t6:t2 0x84:rt:t7:t9 0x88:rs:t0:t4 0x88:rt:v0:t8 0x90:rs:v0:t8 0x90:rt:v1:t6 0x98:rs:v1:t6 */
#include "types.h"
extern void sub_sub_step_up(void);
extern s32 serifu_no;
extern s16 now_serifu;
extern s32 ss_serifu;

typedef struct {
    u8 b[3];
} Blk3;

extern Blk3 date2_serifu_prefix_set_tbl;

void date2_serifu_prefix_set(void)
{
    Blk3 buf;
    s32 v0;
    s32 t2;
    s32 idx;
    s32 base;
    u8 **pp;
    u8 *p;

    v0 = serifu_no;
    buf = date2_serifu_prefix_set_tbl;
    t2 = (v0 << 1) + 13;
    now_serifu = (s16)t2;
    if (v0 == 0) {
        idx = (s16)t2;
        base = ss_serifu;
        pp = *(u8 ***)(base + idx * 4 + 4);
        p = *pp;
        p[0] = buf.b[0];
        idx = now_serifu;
        base = ss_serifu;
        pp = *(u8 ***)(base + idx * 4 + 4);
        p = *pp;
        p[1] = buf.b[1];
    }
    sub_sub_step_up();
}
