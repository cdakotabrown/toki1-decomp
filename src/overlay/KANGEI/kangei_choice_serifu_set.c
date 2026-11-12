/* @CompileEnv: IDO52_CONST_INREG_VALUES=1 */
/* kangei_choice_serifu_set - KANGEI overlay, Address: 0x80136978, Size: 180 bytes (45 insns) */
/* @SymbolSize: 180 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x7C */
/* @PostProc: swap_instructions --offset1 0x7C --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x88 */
/* @PostProc: patch_insn_reg_field 0x30:rt:t6:t7 0x34:rs:t6:t7 0x34:rt:t6:t7 0x38:rt:a0:v1 0x3c:rd:a0:v1 0x3c:rs:a0:v1 0x3c:rt:t6:t7 0x40:rs:a0:v1 0x40:rt:a0:v1 0x44:rt:a2:a1 0x48:rs:a0:v1 0x48:rt:t7:t8 0x4c:rs:t7:t8 0x54:rs:a2:a1 0x54:rt:a0:v1 0x58:rt:t1:t0 0x64:rd:t2:t1 0x64:rt:t1:t0 0x68:rd:a1:t2 0x68:rt:t2:t1 0x6c:rs:a1:t2 0x6c:rt:a1:t3 0x78:rt:v1:a0 0x7c:rs:v1:a0 0x84:rs:v1:a0 0x84:rt:v1:a0 0x88:rt:a1:t3 0x8c:rs:v1:a0 0x8c:rt:t3:t6 0x94:rs:a2:t6 0x94:rt:t3:a1 0x98:rt:t3:t6 0x9c:rs:a0:v1 0xa0:rs:a1:t3 0xa0:rt:t4:t6 0xa8:rt:t4:t6 */
#include "types.h"

extern s32 srand(s32 a0);
extern s32 rand(void);
extern void sub_sub_step_up(void);

extern s32 frame_counter;
extern u8 kangei_choice_no;
extern u8 kangei_choice_tbl[];
extern s16 kangei_now_s;
extern u8 kangei_choice_rnd;

void kangei_choice_serifu_set(void)
{
    s32 v0;
    u32 idx;
    u32 entry;
    u32 one;
    s32 combo;
    s32 a0v;
    u32 t6;

    srand(frame_counter);
    v0 = rand();

    idx = kangei_choice_no;
    entry = kangei_choice_tbl[idx];

    one = 1;
    a0v = v0 % 2;
    combo = (s32)((entry ^ 2) == 0) * 6 +
            (s32)((one ^ entry) == 0) * 4 + 4;

    t6 = (u32)a0v & 0xFFU;
    kangei_now_s = (s16)combo;
    kangei_choice_rnd = (u8)t6;
    if (t6 == one && entry == 0) {
        kangei_now_s = (s16)(combo + 2);
    }
    sub_sub_step_up();
}
