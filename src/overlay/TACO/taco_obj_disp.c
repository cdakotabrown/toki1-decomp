/* @CompileFlags: -Wo,-loopunroll,0 */
/* @SymbolSize: 248 */
/* @PostProc: pad_trailing_nop --count 2 */
/* @PostProc: patch_stack_complete --old 128 --new 152 */
/* @PostProc: remap_stack_slots --from 92 --to 80 --from 60 --to 112 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x5c */
/* Address: 0x80144688 */
#include "types.h"

typedef struct { s32 w[4]; } E16;
typedef struct { s32 w[5]; } E20;

extern u8 scene_sub_b;
extern u8 ref_view[];
extern E16 taco_obj[];
extern E16 taco_coord[];
extern E20 ot_work[];
extern volatile s32 activeBuff;

extern void GsSetRefView2();
extern void GsGetLws();
extern void GsSetLightMatrix();
extern void GsGetLs();
extern void GsSetLsMatrix();
extern void GsSortObject4();

void
taco_obj_disp(void)
{
    E16 *p;
    s32 m1[8];
    s32 m2[8];

    if ((u8)scene_sub_b == 0) {
        return;
    }
    GsSetRefView2(ref_view);
    p = taco_obj;
    do {
        GsGetLws(p->w[1], m1, m2);
        GsSetLightMatrix(m1);
        GsGetLs(p->w[1], m2);
        GsSetLsMatrix(m2);
        GsSortObject4(p, &ot_work[activeBuff], 6, (void *)0x1F800000);
        p++;
    } while (p != taco_coord);
}
