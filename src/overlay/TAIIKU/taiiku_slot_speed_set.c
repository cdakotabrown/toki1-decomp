/* @SymbolStart: 0x801472BC */
/* @SymbolSize: 240 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --from 20 --to 32 */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xb4 */
/* @PostProc: patch_insn_reg_field 0xa0:rd:t8:t6 0xac:rt:t6:t7 0xb0:rt:t9:t5 0xb4:rt:t5:t9 0xb8:rd:a3:a2 0xb8:rs:t8:t6 0xb8:rt:t6:t7 0xd0:rs:a3:a2 0xd0:rt:a2:a3 0xd8:rs:a2:a3 0xe4:rt:a2:a3 */
#include "types.h"

typedef struct { s32 w[3]; } Rec12;
typedef struct {
    u8 b0; u8 b1; u8 pad[10];
    u32 w12; s32 w16; s32 w20;
    u8 pad2[4];
} Rec28;

extern Rec12 taiiku_slot_speed_set_tbl;
extern Rec28 taiiku_spoon_cpu[];
extern s16 cw104_x;
extern u8 class_w[][68];

void taiiku_slot_speed_set(void) {
    Rec12 buf;
    s32 i;
    s32 sum;
    s32 v;

    buf = taiiku_slot_speed_set_tbl;

    for (i = 0; i != 3; i++) {
        if (taiiku_spoon_cpu[i].b1 == 1) {
            if ((cw104_x + 48) < *(s16 *)(class_w[i] + 6634)) {
                taiiku_spoon_cpu[i].w20 = 0;
                continue;
            }
        }
        if (taiiku_spoon_cpu[i].w12 < 11U) {
            continue;
        }
        sum = taiiku_spoon_cpu[i].w20 + taiiku_spoon_cpu[i].w16;
        taiiku_spoon_cpu[i].w20 = sum;
        if (sum < 0) {
            taiiku_spoon_cpu[i].w20 = 0;
            sum = 0;
        }
        v = buf.w[i];
        if (v < sum) {
            taiiku_spoon_cpu[i].w20 = v;
        }
        taiiku_spoon_cpu[i].w12 = 0;
    }
}
