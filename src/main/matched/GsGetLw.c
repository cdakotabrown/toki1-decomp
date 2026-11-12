/* Function: GsGetLw */
/* Address: 0x8009E930 */
/* Size: 568 bytes (142 instructions) */
/* Region: Library (0x80086000+) */
/* Likely Symbol: GsGetLw / SsSepInit */
/* @Compiler: gcc260 */
/* @CompileFlags: -O1 --no-cross-jumping --fill-earliest --fill-load-delay --load-dest-temp --macro-lui-fill --jal-load-nop */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_registers --from s0 --to s1 --bidirectional --exclude-offsets 0x00c,0x010,0x014,0x224,0x228,0x22c */
/* @PostProc: swap_registers --from s1 --to s2 --bidirectional --exclude-offsets 0x00c,0x010,0x014,0x224,0x228,0x22c */
/* @PostProc: swap_instructions 0x4 0xc */
/* @PostProc: swap_instructions 0x8 0x10 */
/* @PostProc: swap_instructions 0xc 0x18 */
/* @PostProc: swap_instructions 0x10 0x18 */
/* @PostProc: swap_instructions 0x14 0x24 */
/* @PostProc: swap_instructions 0x1c 0x28 */
/* @PostProc: swap_instructions 0x20 0x28 */
/* @PostProc: swap_instructions 0x1a4 0x1a8 */
/* @PostProc: swap_instructions 0x1a8 0x1ac */
/* @PostProc: patch_insn_reg_field 0xd8:rt:v1:v0 0xdc:rs:v1:v0 0xdc:rt:v1:v0 0x14c:rt:v1:at 0x150:rs:v1:at 0x150:rd:v1:at 0x154:rs:v1:at 0x154:rt:v1:v0 0x15c:rs:v1:v0 0x15c:rt:v0:v1 0x160:rs:v1:v0 0x164:rs:v1:v0 0x168:rs:v1:v0 0x16c:rt:v0:v1 0x17c:rs:v1:v0 0x180:rs:v1:v0 0x184:rs:v1:v0 0x188:rs:v1:v0 0x188:rt:v0:v1 0x198:rt:v0:v1 0x1a4:rt:v0:v1 0x1a8:rs:v0:v1 0x1a8:rt:v0:v1 0x1ac:rd:v1:v0 0x1b0:rs:v1:v0 0x1b0:rt:v0:v1 */
#include "types.h"

extern void GsMulCoord3(s32 *out, s32 *src);

extern s32 *DSTACK[100];

void GsGetLw(s32 *arg0, s32 *output) {
    s32 counter;
    s32 final_idx;
    s32 *v;
    s32 **arrp;
    s32 **pp;
    s32 *base;
    s32 one;

    counter = 0;
    final_idx = 100;
    arrp = &DSTACK[0];

top:
    *arrp = arg0;
    v = (s32 *)arg0[18];
    if (v == 0) {
        if (arg0[0] == 0) {
            final_idx = counter;
        }
        goto after;
    }
    if (arg0[0] == 0) {
        final_idx = counter;
    }
    arg0 = v;
    arrp++;
    counter++;
    goto top;

after:
    if (counter == 0) {
        s32 a, b, c, d;
        s32 *w;
        w = DSTACK[0];
        a = w[1]; b = w[2]; c = w[3]; d = w[4];
        output[0] = a; output[1] = b; output[2] = c; output[3] = d;
        d = w[8]; c = w[7]; b = w[6]; a = w[5];
        output[7] = d; output[6] = c; output[5] = b;
        output[4] = a;
        goto done_copy;
    }
    if (final_idx == 100) {
        v = DSTACK[0];
        counter = 0;
        goto copy_second;
    }
    if (final_idx == counter) {
        s32 a, b, c, d;
        s32 *w;
        w = DSTACK[counter];
        a = w[1]; b = w[2]; c = w[3]; d = w[4];
        output[0] = a; output[1] = b; output[2] = c; output[3] = d;
        d = w[8]; c = w[7]; b = w[6]; a = w[5];
        output[7] = d; output[6] = c; output[5] = b;
        output[4] = a;
        goto done_copy;
    }

    v = DSTACK[final_idx + 1];
    counter = final_idx + 1;

copy_second:
    {
    s32 a, b, c, d;
    a = v[9]; b = v[10]; c = v[11]; d = v[12];
    output[0] = a; output[1] = b; output[2] = c; output[3] = d;
    d = v[16]; c = v[15]; b = v[14]; a = v[13];
    output[7] = d; output[6] = c; output[5] = b;
    output[4] = a;
    }

done_copy:
    if (counter > 0) {
        one = 1;
        pp = &DSTACK[counter - 1];
        do {
            s32 a, b, c, d;
            GsMulCoord3(output, (*pp) + 1);
            base = *pp;
            a = output[0]; b = output[1]; c = output[2]; d = output[3];
            base[9] = a; base[10] = b; base[11] = c; base[12] = d;
            d = output[7]; c = output[6]; b = output[5]; a = output[4];
            base[16] = d; base[15] = c; base[14] = b; base[13] = a;
            base = *pp;
            counter--;
            pp--;
            base[0] = one;
        } while (counter > 0);
    }
}
