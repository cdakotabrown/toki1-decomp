/* Address: 0x800909E0 */
/* Size: 152 bytes (38 instructions) */
/* Region: Library (0x80086000+) */
/* Compiler: IDO 5.2 -O2 codegen, PSY-Q-style assembly (@Compiler: ido52-gas) */
/* @Compiler: ido52-gas */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x10 */
#include "types.h"

typedef struct {
    u8 pad[144];
    u32 flags;
} Voice;

extern Voice *_ss_score[];

void SsSeqReplay(s16 v)
{
    Voice *p = _ss_score[v];
    u32 flags = p->flags;

    if ((flags & 4) || (flags & 0x200) || (flags & 0x100)) {
        return;
    }
    _ss_score[v]->flags &= ~2;
    _ss_score[v]->flags |= 8;
    _ss_score[v]->flags |= 1;
}
