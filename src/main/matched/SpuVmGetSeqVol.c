/* Address: 0x8008E7D0 */
/* Size: 128 bytes (32 instructions) */
/* Region: Library (0x80086000+) */
/* (@Compiler: ido52-gas).  Evidence: retail leaves a load-delay `nop` */
/* @Compiler: ido52-gas */
/* @CompileFlags: --ido-copy-and-bump --ido-fill-load-delay --ido-fill-macro-lui */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u16 id;
    u16 w;
    u16 h;
    u16 pad;
} Ent;

extern s16 D_80131606;
extern Ent libsnd_seq_vol_tbl[];

s32 SpuVmGetSeqVol(s16 n, u16 *w, u16 *h)
{
    D_80131606 = n & 0xFF;
    if (D_80131606 < 34) {
        *w = libsnd_seq_vol_tbl[D_80131606].w;
        *h = libsnd_seq_vol_tbl[D_80131606].h;
        return D_80131606;
    }
    return -1;
}
