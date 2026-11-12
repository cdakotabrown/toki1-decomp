/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t9:t0 0x54:rt:t0:t1 0x58:rt:t0:t1 0x5c:rt:t1:t2 0x60:rs:t1:t2 0x60:rt:t1:t2 0x68:rt:t1:t2 0x6c:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t2:t3 0x78:rt:t2:t3 0x7c:rt:t3:t4 0x80:rs:t3:t4 0x80:rt:t3:t4 0x88:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rt:t4:t5 0x9c:rt:t5:t6 0xa0:rs:t5:t6 0xa0:rt:t5:t6 0xa8:rt:t5:t6 0xac:rt:t6:t7 0xb0:rs:t6:t7 0xb0:rt:t6:t7 0xbc:rt:t6:t7 */
/* Address: 0x80135820 */
/* Size: 176 bytes (44 insns) */
#include "types.h"
#include "tokimeki/etc_ev_res.h"

extern s16 etc_ev47_cel_scale;
extern ETC_EV_RES etc_ev41_res;

void etc_ev41_res_init(void)
{
    etc_ev41_res.hdr = 0x801CE0F4;
    etc_ev41_res.cels = 0x801CE0F8;
    etc_ev41_res.anim_tbl = 0x801CE118;
    etc_ev41_res.scale = etc_ev47_cel_scale;
    etc_ev41_res.pal = 0x801B0000;
    etc_ev41_res.bg[0] = 0x801B2000;
    etc_ev41_res.bg[1] = 0x801B6000;
    etc_ev41_res.bg[2] = 0x801BA000;
    etc_ev41_res.bg[3] = 0x801BE000;
    etc_ev41_res.bg[4] = 0x801C2000;
    etc_ev41_res.bg[5] = 0x801C6000;
}
