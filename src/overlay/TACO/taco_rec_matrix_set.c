/* taco_rec_matrix_set -- TACO overlay, Address: 0x8015B1D8 */
/* - `@CompileEnv: IDO52_LDA_NOINREG_CLASSES=1` (sp-LDA remat): splits */
/* @CompileEnv: IDO52_LDA_NOINREG_CLASSES=1 */
/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: patch_insn_reg_field 0x2c:rt:v1:t0 0x38:rd:v0:v1 0x38:rs:v1:t0 0x3c:rt:t0:a3 0x50:rd:a0:v0 0x54:rd:a3:a0 0x54:rs:v0:v1 0x5c:rt:a0:v0 0x5c:rs:a0:v0 0x60:rt:a3:a0 0x60:rs:a3:a0 0x68:rs:a0:v0 0x6c:rs:a3:a0 0x74:rt:v1:t0 0x74:rs:v1:t0 0x78:rt:v0:v1 0x78:rs:v0:v1 0x80:rs:v1:t0 0x80:rt:t0:a3 0x84:rs:v1:t0 0xa8:rt:t2:t3 0xb0:rt:t2:t3 0xc0:rt:t2:t3 0xc8:rt:t2:t3 0xd8:rt:t2:t3 0xe0:rt:t2:t3 0xf0:rt:t2:t3 0x100:rt:t2:t3 */
/* @SymbolStart: 0x8015B1C8 */
/* @SymbolSize: 244 */
#include "types.h"

extern u8 taco_coord[];

typedef struct {
    s16 s[9];
    s32 w[3];
} Buf;

typedef struct {
    s32 zero;
    Buf payload;
} Ent;

void taco_rec_matrix_set(s32 idx, s16 *a1, s16 *a2, s32 row)
{
    s32 pad[2];
    Buf buf;
    s16 *v1;
    s16 *a0p;
    s32 *t0p;
    s32 v0;
    u8 *base;
    s32 *end;
    Ent *out;

    a2 += row * 9;
    a1 += row * 3;
    base = taco_coord;
    v1 = (s16 *)&buf;
    t0p = (s32 *)v1;
    end = (s32 *)((char *)&buf + 12);
    do {
        v0 = 0;
        a0p = v1;
        do {
            *a0p = *a2;
            v0 += 1;
            a0p++;
            a2++;
        } while (v0 != 3);
        t0p += 1;
        *(s32 *)((char *)t0p + 16) = (s32)*a1;
        v1 += 3;
        a1++;
    } while (t0p != end);

    out = (Ent *)((u32)base + idx * 80);
    out->payload = buf;
    out->zero = 0;
}
