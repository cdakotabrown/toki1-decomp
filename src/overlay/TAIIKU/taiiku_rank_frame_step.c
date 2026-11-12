/* @PostProc: patch_insn_reg_field   0x90:rd:t1:t3 0x9c:rt:t1:t3 0xbc:rt:t2:t4 0xc0:rs:t2:t4 0xc0:rt:t2:t4 0xc8:rs:t2:t4 0xc8:rt:t3:t5 0xcc:rd:t4:t8 0xcc:rs:t3:t5 0xd8:rt:t4:t8 */
/* @SymbolStart: 0x801388EC */
/* @SymbolSize: 228 */
/* Address: 0x801388F4 */
/* Size: 220 bytes (55 insns) */
#include "types.h"
#include "tokimeki/taiiku_kari_run.h"

extern s16 class_w[];
extern TAIIKU_KARI_RUN taiiku_kari_run[];

extern s32 get_taiiku_rank_frame(s32, s32);

void taiiku_rank_frame_step(void)
{
    s32 i;
    s32 val;

    if ((u32)taiiku_kari_run[0].cnt0c >= 11) {
        taiiku_kari_run[0].frm++;
        taiiku_kari_run[0].cnt0c = 0;
    }

    if ((u32)taiiku_kari_run[0].frm >= 4U) {
        taiiku_kari_run[0].frm = 0;
    }

    for (i = 0; i < 4; i++) {
        val = get_taiiku_rank_frame(i, 1);
        class_w[3276 + i * 34] = taiiku_kari_run[0].frm + val;
    }

    for (i = 0; i < 4; i++) {
        val = get_taiiku_rank_frame(i, 0);
        class_w[3650 + i * 34] = (taiiku_kari_run[0].frm & 1) + val;
    }
}
