/* Function: SetDrawArea */
/* Address: 0x800A0C3C */
/* Size: 144 bytes (36 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 24 --new 32 --shift-ra */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=0:a2,6:v1 */
#include "types.h"

typedef struct {
    s16 x, y, w, h;
} Rect;

extern s32 get_cs(s32, s32);
extern s32 get_ce(s32, s32);

void SetDrawArea(volatile s32 *ptr, Rect *rect) {
    *ptr = (*ptr & 0x00FFFFFF) | 0x02000000;
    ptr[1] = get_cs(rect->x, rect->y);
    ptr[2] = get_ce(rect->x + rect->w - 1, rect->y + rect->h - 1);
}
