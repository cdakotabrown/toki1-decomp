/* Address: 0x8015B82C */
/* Size: 216 bytes (54 insns) */
/* @PostProc: patch_stack_complete --old 80 --new 88 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xbc */
/* @PostProc: patch_insn_reg_field 0xa4:rt:a0:t8 0xa8:rd:a0:t8 0xa8:rs:a0:t8 0xac:rs:a0:t8 0xac:rt:a0:t8 0xb8:rd:t8:t7 0xb8:rt:a0:t8 0xbc:rt:t8:t7 */
#include "types.h"

typedef struct { s32 w[16]; } Tbl16;
typedef struct { s32 f0; s8 pad[52]; } Ent56;

extern Tbl16 get_bunkasai_table_tbl;
extern s32   bunka_girl;
extern u8    girl2_basyo;
extern u16   basyo_pack;
extern Ent56 girl0_birth[];
extern s32   p_birthday;

s32 get_bunkasai_table(void) {
    Tbl16 local;
    s32 sel;
    s32 a;
    s32 b;
    s32 word;

    local = get_bunkasai_table_tbl;
    sel = bunka_girl;
    if (sel == 2 && ((u32)girl2_basyo >> 4) == 3 && ((u32)basyo_pack >> 12) != 3) {
        return 3;
    }
    word = girl0_birth[sel].f0;
    a = (u32)(word << 21) >> 30;
    b = (u32)(p_birthday << 21) >> 30;
    return local.w[a * 4 + b];
}
