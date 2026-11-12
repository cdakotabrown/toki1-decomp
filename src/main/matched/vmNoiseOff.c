/* Address: 0x8008D548 */
/* Size: 56 bytes (14 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -O2 */
/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0x8 */
/* @PostProc: patch_insn_reg_field 0x0:rt:t6:a0 0x4:rt:t6:a0 0x4:rd:t7:t6 0x8:rt:v0:v1 0xc:rs:v0:v1 0xc:rt:v0:v1 0x10:rs:t7:t6 0x10:rt:t6:a0 0x10:rd:t7:t6 0x14:rt:t8:t7 0x18:rs:t8:t7 0x18:rt:t8:t7 0x1c:rt:t7:t6 0x1c:rd:t7:t6 0x20:rs:t7:t6 0x20:rt:t8:t7 0x20:rd:v1:v0 0x24:rs:v1:v0 0x28:rs:v1:v0 0x2c:rs:v0:v1 0x34:rs:v0:v1 */
#include "types.h"

typedef struct {
    u8  pad[2];
    s16 f2;
    s16 f4;
    u8  pad2[17];
    u8  f23;
} V24b;

typedef struct {
    u8  pad[404];
    s16 f404;
    s16 f406;
} SpuBlk;

extern V24b _svm_voice[];
extern SpuBlk * volatile libsnd_spu_regs_ptr;

void vmNoiseOff(s32 voice)
{
    SpuBlk *g;
    V24b *e;

    g = libsnd_spu_regs_ptr;
    e = &_svm_voice[voice & 0xFF];
    e->f4 = 0;
    e->f23 = 0;
    g->f406 = 0;
    g->f404 = 0;
}
