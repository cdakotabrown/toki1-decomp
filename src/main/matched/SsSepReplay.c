/* Address: 0x80090A78 */
/* Size: 216 bytes (54 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 codegen, PSY-Q-style assembly */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_LA_PULL=all IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_NO_COPY_PROP=all IDO52_AS1_NO_PASS1_PROP=17 IDO52_FORCE_WEB_COLOR=5:v1 IDO52_UGEN_TEMP_REG=16:t0,17:t1,18:t2 IDO52_UGEN_TEMP_REG_INPLACE=occ:16 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_insn_reg_field 0x94:rd:v0:a2 0x98:rs:v0:a2 0xa8:rs:v0:a2 0xbc:rd:v0:a2 0xc0:rs:v0:a2 0xcc:rs:v0:a2 */
#include "types.h"

typedef struct {
    u8 pad[144];
    u32 flags;
    u8 pad2[20];
} SeqStruct;

extern SeqStruct *_ss_score[];

void SsSepReplay(s16 sep, s16 seq)
{
    SeqStruct **pp = &_ss_score[sep];
    SeqStruct *p = &(*pp)[seq];
    SeqStruct *rec;
    u32 flags = p->flags;

    if ((flags & 4) || (flags & 0x200) || (flags & 0x100)) {
        return;
    }
    rec = &(*pp)[seq];
    rec->flags &= ~2;
    rec = &(*pp)[(u32)seq];
    rec->flags |= 8;
    rec = (SeqStruct *)((u8 *)*pp + ((seq * 21) << 3));
    rec->flags |= 1;
}
