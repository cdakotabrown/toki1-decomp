/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=0 IDO52_AS1_XBB_SKIP_CAND=occ:1 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* Address: 0x8013F5BC */
/* Size: 344 bytes (86 instructions) */
/* @PostProc: patch_stack_complete --old 216 --new 224 */
/* @PostProc: swap_instructions --offset1 0xB4 --offset2 0xC0 */
/* @PostProc: swap_instructions --offset1 0xCC --offset2 0xD0 */
/* @PostProc: patch_insn_reg_field 0x70:rt:t1:a2 0x74:rs:t1:a2 0x74:rt:t1:a2 0x7c:rd:t2:t1 0x7c:rt:t1:a2 0x80:rt:t2:t1 0x9c:rt:t3:t2 0xa0:rd:t4:t3 0xa4:rd:t5:t4 0xa4:rt:t4:t3 0xa8:rs:t5:t4 0xa8:rt:t5:t4 0xac:rs:t3:t2 0xac:rt:t3:t2 0xb0:rs:t3:t2 0xb0:rt:t5:t4 0xcc:rd:t6:t7 0xd0:rt:t8:t5 0xd4:rs:t8:t5 0xd4:rt:t8:t5 0xd8:rt:t6:t7 0xe0:rd:t0:t8 0xe4:rd:t7:t0 0xe4:rt:t0:t8 0xec:rs:t7:t0 0xec:rt:t8:t5 0xf0:rt:v0:t1 0xf4:rs:v0:t1 0xf4:rt:v0:t1 0xfc:rs:v0:t1 0x114:rd:t3:t4 0x118:rt:t3:t4 0x120:rd:t2:t9 0x124:rt:t2:t9 0x128:rt:t9:t3 0x130:rs:t9:t3 0x130:rt:t4:t2 0x138:rt:t4:t2 */
#include "types.h"

typedef void (*FuncPtr)(s32);
typedef struct { FuncPtr ptrs[44]; } Tbl44;

extern Tbl44 D_801228E0;
extern u8 D_8011F8C0[];
extern volatile u8 D_800B1AF6;
extern volatile u8 D_800EAFA7;
extern u8 D_800E9620[];

extern void func_8004C984(s32);
extern void func_80078950(u8 *, s32);
extern void func_80103774(void);

void func_8013F5BC(void) {
    Tbl44 tbl;
    u8 charId;
    u32 g;
    s32 i;

    tbl = D_801228E0;

    func_80078950(D_8011F8C0, D_800B1AF6);

    tbl.ptrs[D_800B1AF6](128);

    charId = D_800B1AF6;
    if (*(FuncPtr *)((u8 *)tbl.ptrs + (charId << 2)) == func_8004C984) {
        i = 0;
        do {
            D_800E9620[i * 68 + 6671] = D_800EAFA7;
            i = (s16)(i + 1);
        } while (i < 8);
        g = D_800EAFA7;
        if (g < 9U) {
            for (i = 0; i < 8; i = (s16)(i + 1)) {
                D_800E9620[i * 68 + 6667] = D_800E9620[i * 68 + 6667] & 0x7F;
            }
        }
    }
    if ((u32)charId >= 17U && (u32)charId < 20U) {
        func_80103774();
    }
}
