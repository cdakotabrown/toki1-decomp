/* Address: 0x8007B46C */
/* Size: 132 bytes (33 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: strip_trailing_instructions --count 3 --nop-only */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_insn_reg_field 0x4:rt:t6:t7 0x8:rt:v0:a2 0xc:rs:v0:a2 0xc:rt:t9:t0 0x10:rt:t9:t0 0x10:rd:t0:t1 0x14:rt:t6:t7 0x14:rd:t7:t8 0x1c:rs:t7:t8 0x1c:rt:t8:t9 0x20:rs:t0:t1 0x20:rt:t1:t2 0x24:rs:v0:a2 0x24:rt:t2:t3 0x2c:rs:t2:t3 0x30:rs:t8:t9 0x30:rt:t1:t2 0x30:rd:a2:a3 0x34:rs:v0:a2 0x38:rd:t3:t4 0x3c:rs:t3:t4 0x40:rd:t4:t5 0x44:rs:t4:t5 0x4c:rs:a2:a3 0x58:rs:v0:a2 0x5c:rd:t6:t7 0x60:rs:t6:t7 0x64:rd:t7:t8 0x68:rs:t7:t8 0x70:rs:a2:a3 */
#include "types.h"

extern u16 sd_vab_ch3;
extern void voice_off(u32, u32);
extern void voice_restart(u32, u32);

void cmd_voice6(u32 a0) {
    u32 a2;
    u32 color;
    u32 yoff;
    u32 bioVal;

    a2 = a0 & 0xFFFF;
    bioVal = sd_vab_ch3;

    color = ((bioVal << 8) & 0xFF00) | (((a2 & 0xFF) >> 4) & 0xFF);

    if (a2 & 0x1000) {
        yoff = (((a2 & 0xF) * 4 + 24) << 8) & 0xFF00;
        voice_off(color, yoff);
    } else {
        yoff = (((a2 & 0xF) * 4 + 24) << 8) & 0xFF00;
        voice_restart(color, yoff);
    }
}
