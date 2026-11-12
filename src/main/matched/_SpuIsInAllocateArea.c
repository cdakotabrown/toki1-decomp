/* Address: 0x80098408 */
/* Size: 136 bytes (34 instructions) */
/* Region: Library */
/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=1 IDO52_AS1_NO_LA_PULL=1 IDO52_FORCE_WEB_COLOR=15:a1 IDO52_AS1_NO_COPY_PROP=0 IDO52_AS1_KEEP_DEAD_RELOAD=0 IDO52_AS1_TIE_TAKE=17 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct { u32 flags; u32 extra; } Node;
extern Node *_SpuMemList;

s32 _SpuIsInAllocateArea(u32 addr) {
    s32 i;
    u32 v;
    u32 size;

    for (i = 0;; i++) {
        v = _SpuMemList[i].flags;
        if (!(v & 0x80000000)) {
            if (v & 0x40000000) break;
            size = v & 0x0FFFFFFF;
            if (addr <= size) return 1;
            if (addr < _SpuMemList[i].extra + (0, size)) return 1;
        }
    }
    return 0;
}
