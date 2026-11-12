/* Address: 0x8007B344 */
/* Size: 164 bytes (41 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x20 */
/* @PostProc: patch_insn_reg_field 0x2c:rt:v1:v0 0x30:rs:v1:v0 0x3c:rs:v1:v0 0x48:rs:v1:v0 */
#include "types.h"

extern s32 gSDSerialInfo;
extern s16 seq_fade_acc;
extern s16 seq_fade_lim;
extern s16 seq_fade_step;

void seq_vol_fade_set(u16 arg0) {
    u32 cmd;

    if (gSDSerialInfo & 1) {
        return;
    }
    seq_fade_acc = 0;
    gSDSerialInfo |= 1;

    cmd = arg0 & 0xFF;
    switch (cmd) {
    case 177:
        goto c177;
    case 193:
        goto c193;
    case 209:
        goto c209;
    }
    return;
c177:
    seq_fade_lim = 256;
    seq_fade_step = 10;
    return;
c193:
    seq_fade_lim = 192;
    seq_fade_step = 1;
    return;
c209:
    seq_fade_lim = 128;
    seq_fade_step = 1;
}
