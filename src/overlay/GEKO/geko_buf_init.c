/* @PostProc: scheduler_normalizer --const-fp --fence 16-18,19-22,23-26,27-30,31-34,35-38,39-43 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t9:t0 0x54:rt:t0:t1 0x58:rt:t0:t1 0x5c:rt:t1:t2 0x60:rs:t1:t2 0x60:rt:t1:t2 0x68:rt:t1:t2 0x6c:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t2:t3 0x78:rt:t2:t3 0x7c:rt:t3:t4 0x80:rs:t3:t4 0x80:rt:t3:t4 0x88:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rt:t4:t5 0x9c:rt:t5:t6 0xa0:rs:t5:t6 0xa0:rt:t5:t6 0xa8:rt:t5:t6 0xac:rt:t6:t7 0xb0:rs:t6:t7 0xb0:rt:t6:t7 0xbc:rt:t6:t7 */
/* Address: 0x80140B10 */
/* Size: 176 bytes (44 insns) */
#include "types.h"

extern s16 geko_scene77_scale_src;
extern s32 geko_scene77_res[];

void geko_buf_init(void)
{
    geko_scene77_res[0] = 0x801CE150;
    geko_scene77_res[1] = 0x801CE154;
    geko_scene77_res[2] = 0x801CE1A4;
    (*(s16 *)&geko_scene77_res[3]) = geko_scene77_scale_src;
    geko_scene77_res[4] = 0x801B0000;
    geko_scene77_res[5] = 0x801B2000;
    geko_scene77_res[6] = 0x801B6000;
    geko_scene77_res[7] = 0x801BA000;
    geko_scene77_res[8] = 0x801BE000;
    geko_scene77_res[9] = 0x801C2000;
    geko_scene77_res[10] = 0x801C6000;
}
