/* date_basyo18_en_nichi_face -- DATE overlay, Address: 0x8013A3B4 */
/* @SymbolStart: 0x8013A3AC */
/* @SymbolSize: 224 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=2 */
/* @PostProc: swap_branch_operands --offsets 0x90,0xB8 */
#include "types.h"

extern void bustup_face_change(s32 arg);
extern void sub_sub_step_up(void);
extern u8 now_year;
extern s32 en_nichi_res;
extern u8 now_girl;

void date_basyo18_en_nichi_face(void)
{
    s32 sel;
    s32 g;
    s32 h;

    sel = now_year;
    if (sel != 95) {
        goto l_A3D4;
    }
    bustup_face_change(1);
    goto l_A474;
l_A3D4:
    g = en_nichi_res;
    if (g != 0) {
        goto l_A3F8;
    }
    bustup_face_change(1);
    goto l_A474;
l_A3F8:
    if (g != 1) {
        goto l_A414;
    }
    bustup_face_change(2);
    goto l_A474;
l_A414:
    if (g != 2) {
        goto l_A42C;
    }
    bustup_face_change(3);
    goto l_A474;
l_A42C:
    if (3 != g) {
        goto l_A454;
    }
    h = now_girl;
    if (h == 2) {
        goto l_A454;
    }
    bustup_face_change(0);
    goto l_A474;
l_A454:
    if (3 != g) {
        goto l_A46C;
    }
    bustup_face_change(1);
    goto l_A474;
l_A46C:
    bustup_face_change(3);
l_A474:
    sub_sub_step_up();
}
