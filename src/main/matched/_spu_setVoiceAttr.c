/* Address: 0x80096A18 */
/* Size: 464 bytes (116 instructions) */
/* Region: Library (0x80086000+) */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_AS1_DIV_MACRO=aspsx IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_FORCE_WEB_COLOR=0:v1,5:a1,25:t0,45:t3 IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_NO_SHIFT_COMBINE=all IDO52_AS1_FORCE_BOPT=2 IDO52_AS1_KEEP_ANDI=all IDO52_COPYPROP_SITE=lod:1:deny */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

typedef struct {
    u32 mask;
    u32 attr;
    u16 volL;
    u16 volR;
    u16 pad0C[2];
    u16 pitch;
    u16 pad12;
    u32 addr;
    u32 loop;
    u32 adsr;
} VAttr;

extern u16 *_spu_RXX;
extern s32 spu_addrMode;

void _spu_setVoiceAttr(VAttr *a) {
    u16 *base;
    u32 attr;
    u32 shift;
    s32 all;
    s32 mode;
    s32 i;
    u32 raw;
    s32 idx;

    base = _spu_RXX;
    attr = a->attr;
    shift = (u32)(base[0xD6] & 0xE) >> 1;
    if (attr == 0) {
        all = 1;
    } else {
        all = 0;
    }
    mode = spu_addrMode;
    for (i = 0; i != 24; i++) {
        if (a->mask & (1 << i)) {
            if (all || (attr & 1)) {
                idx = i * 8;
                base[idx + 0] = a->volL;
            }
            if (all || (attr & 2)) {
                idx = i * 8;
                base[idx + 1] = a->volR;
            }
            if (all || (attr & 4)) {
                idx = i * 8;
                base[idx + 2] = a->pitch;
            }
            if (all || (attr & 8)) {
                idx = i * 8;
                if (mode == 0) {
                    base[idx + 3] = a->addr;
                } else {
                    raw = a->addr;
                    if (shift != 0 && (raw % shift) != 0) {
                        raw += shift;
                    }
                    base[idx + 3] = (raw >> (shift + 1)) & 0xFFFF;
                }
            }
            if (all || (attr & 0x10)) {
                idx = i * 8;
                base[idx + 4] = (a->adsr >> 16) & 0xFFFF;
            }
            if (all || (attr & 0x10)) {
                idx = i * 8;
                base[idx + 5] = a->adsr & 0xFFFF;
            }
            if (all || (attr & 0x100)) {
                idx = i * 8;
                base[idx + 7] = a->loop & 0xFFFF;
            }
        }
    }
}
