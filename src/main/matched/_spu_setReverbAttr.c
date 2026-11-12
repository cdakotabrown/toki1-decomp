/* Address: 0x80096BE8 */
/* Size: 1084 bytes (271 instructions) */
/* Region: Library (0x80086000+) */
/* Compiler: IDO 5.2 -O2 codegen, PSY-Q-style assembly (@Compiler: ido52-gas) */
/* @Compiler: ido52-gas */
/* @CompileFlags: --ido-fill-load-delay --ido-copy-and-bump */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u32 mask;
    u16 v[32];
} SpuReverbRegs;

extern volatile u16 * volatile _spu_RXX;

s32 _spu_setReverbAttr(SpuReverbRegs *attr)
{
    u32 mask;
    s32 all;
    volatile u16 *r;

    mask = attr->mask;
    if (mask == 0) {
        all = 1;
    } else {
        all = 0;
    }

    if (!(all || (mask & 0x1))) {
        r = _spu_RXX;
    } else {
        r = _spu_RXX;
        r[0xE0] = attr->v[0];
    }
    if (all || (mask & 0x2)) {
        r[0xE1] = attr->v[1];
    }
    if (all || (mask & 0x4)) {
        r[0xE2] = attr->v[2];
    }
    if (all || (mask & 0x8)) {
        r[0xE3] = attr->v[3];
    }
    if (all || (mask & 0x10)) {
        r[0xE4] = attr->v[4];
    }
    if (all || (mask & 0x20)) {
        r[0xE5] = attr->v[5];
    }
    if (all || (mask & 0x40)) {
        r[0xE6] = attr->v[6];
    }
    if (all || (mask & 0x80)) {
        r[0xE7] = attr->v[7];
    }
    if (all || (mask & 0x100)) {
        r[0xE8] = attr->v[8];
    }
    if (all || (mask & 0x200)) {
        r[0xE9] = attr->v[9];
    }
    if (all || (mask & 0x400)) {
        r[0xEA] = attr->v[10];
    }
    if (all || (mask & 0x800)) {
        r[0xEB] = attr->v[11];
    }
    if (all || (mask & 0x1000)) {
        r[0xEC] = attr->v[12];
    }
    if (all || (mask & 0x2000)) {
        r[0xED] = attr->v[13];
    }
    if (all || (mask & 0x4000)) {
        r[0xEE] = attr->v[14];
    }
    if (all || (mask & 0x8000)) {
        r[0xEF] = attr->v[15];
    }
    if (all || (mask & 0x10000)) {
        r[0xF0] = attr->v[16];
    }
    if (all || (mask & 0x20000)) {
        r[0xF1] = attr->v[17];
    }
    if (all || (mask & 0x40000)) {
        r[0xF2] = attr->v[18];
    }
    if (all || (mask & 0x80000)) {
        r[0xF3] = attr->v[19];
    }
    if (all || (mask & 0x100000)) {
        r[0xF4] = attr->v[20];
    }
    if (all || (mask & 0x200000)) {
        r[0xF5] = attr->v[21];
    }
    if (all || (mask & 0x400000)) {
        r[0xF6] = attr->v[22];
    }
    if (all || (mask & 0x800000)) {
        r[0xF7] = attr->v[23];
    }
    if (all || (mask & 0x1000000)) {
        r[0xF8] = attr->v[24];
    }
    if (all || (mask & 0x2000000)) {
        r[0xF9] = attr->v[25];
    }
    if (all || (mask & 0x4000000)) {
        r[0xFA] = attr->v[26];
    }
    if (all || (mask & 0x8000000)) {
        r[0xFB] = attr->v[27];
    }
    if (all || (mask & 0x10000000)) {
        r[0xFC] = attr->v[28];
    }
    if (all || (mask & 0x20000000)) {
        r[0xFD] = attr->v[29];
    }
    if (all || (mask & 0x40000000)) {
        r[0xFE] = attr->v[30];
    }
    if (all || (mask & 0x80000000)) {
        r[0xFF] = attr->v[31];
    }
    return 0;
}
