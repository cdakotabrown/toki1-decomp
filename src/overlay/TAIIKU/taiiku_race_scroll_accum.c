/* @SymbolSize: 100 */
/* @PostProc: patch_insn_reg_field 0x18:rt:t7:v0 0x24:rt:t8:t7 0x28:rs:t7:v0 0x28:rt:t7:v0 0x2c:rs:t8:t7 0x2c:rt:t8:t7 0x34:rs:t7:v0 0x34:rt:t8:t7 */
/* Address: 0x80143E6C */
/* Size: 100 bytes (25 insns) */
#include "types.h"

extern s32 taiiku_ninin_run_0_step_cnt;
extern s32 taiiku_ninin_run_0_speed;
extern s32 taiiku_ninin_run_0_accel;
extern s32 taiiku_ninin_vmax;

void taiiku_race_scroll_accum(void)
{
    s32 acc;
    s32 lim;

    if ((u32)taiiku_ninin_run_0_step_cnt < 11U) {
        return;
    }
    acc = taiiku_ninin_run_0_speed + taiiku_ninin_run_0_accel;
    if (acc < -16384) {
        acc = -16384;
    }
    taiiku_ninin_run_0_speed = acc;
    lim = taiiku_ninin_vmax;
    if (lim < acc) {
        taiiku_ninin_run_0_speed = lim;
    }
    taiiku_ninin_run_0_step_cnt = 0;
}
