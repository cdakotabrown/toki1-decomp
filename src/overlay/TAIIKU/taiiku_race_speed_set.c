/* @SymbolStart: 0x80144A80 */
/* @SymbolSize: 204 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: patch_stack_complete --old 40 --new 56 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x8c --offset2 0x90 */
/* @PostProc: patch_insn_reg_field 0x34:rt:t1:t0 0x38:rs:t1:t0 0x38:rt:t1:t0 0x44:rt:t0:t2 0x48:rt:t2:t1 0x64:rt:t2:t1 0x68:rd:t3:t4 0x6c:rt:t4:t5 0x74:rs:t1:t0 0x8c:rt:t7:t6 0x90:rt:t6:t7 0x94:rd:a3:a2 0x94:rs:t3:t4 0x94:rt:t4:t5 0xac:rs:a3:a2 0xac:rt:a2:a3 0xb4:rs:a2:a3 0xc0:rt:a2:a3 0xcc:rt:t0:t2 */
#include "types.h"

typedef struct { s32 w[3]; } Rec12;

extern Rec12 taiiku_race_speed_set_tbl;
extern u8 class_w[][68];
extern u8 taiiku_ninin_run[][36];

void taiiku_race_speed_set(void) {
    Rec12 buf;
    s32 i;
    s32 sum;
    s32 v;

    buf = taiiku_race_speed_set_tbl;

    for (i = 0; i != 3; i++) {
        if ((i + 54) == *(s16 *)(class_w[i] + 6620)) {
            continue;
        }
        if (*(u32 *)(taiiku_ninin_run[i] + 52) < 11U) {
            continue;
        }
        sum = *(s32 *)(taiiku_ninin_run[i] + 48) + *(s32 *)(taiiku_ninin_run[i] + 44);
        *(s32 *)(taiiku_ninin_run[i] + 48) = sum;
        if (sum < 0) {
            *(s32 *)(taiiku_ninin_run[i] + 48) = 0;
            sum = 0;
        }
        v = buf.w[i];
        if (v < sum) {
            *(s32 *)(taiiku_ninin_run[i] + 48) = v;
        }
        *(s32 *)(taiiku_ninin_run[i] + 52) = 0;
    }
}
