/* @CompileEnv: IDO52_LDA_NOINREG_CLASSES=1:1:1 IDO52_COPYPROP_VAR=-36 IDO52_FORCE_WEB_COLOR=0:v1,8:a2,15:a1,22:v1 IDO52_UGEN_TEMP_REG=1:t8,14:t1,15:t0,16:t3,17:t4,19:t7 IDO52_AS1_TIE_TAKE=6,9,12,241 IDO52_AS1_TIE_REJECT=36,37,38,39,40,41 */
/* @SymbolSize: 296 */
/* @CompileFlags: -cckr -Wc,-pic0 */
/* name_hilite_set - NAME_ENT overlay, Address: 0x8013B108, 296 bytes (74 insns) */
#include "types.h"

extern u8 cw117_disp;
extern u8 cw115_disp;
extern u8 cw116_disp;
extern u8 cw118_disp;
extern int name_ent_view_page;

name_hilite_set(mode)
int mode;
{
    int unused0;
    int unused1;
    int flags[3];

    int *p;
    int k0, k1, k2, k3;
    int below0;

    for (p = flags; p < &flags[3]; p++) {
        *p = 0;
    }
    k0 = cw117_disp;
    k1 = cw115_disp;
    k2 = cw116_disp;
    k3 = cw118_disp;
    k0 &= 0x7F;
    k1 &= 0x7F;
    k2 &= 0x7F;
    k3 &= 0x7F;
    if (mode == 0) {
        cw117_disp = k0;
        cw115_disp = k1;
        cw116_disp = k2;
        cw118_disp = k3;
        goto ret0;
ret0:
        return 0;
    }
    flags[name_ent_view_page % 3] = 1;
    cw117_disp = k0;
    if (flags[0]) {
        k2 = (k2 | 0x80) & 0xFF;
        cw117_disp = k0 | 0x80;
    }
    cw115_disp = k1;
    if (flags[1]) {
        cw115_disp = k1 | 0x80;
    }
    cw118_disp = k3;
    cw116_disp = k2;
    if (flags[2]) {
        k3 = (k3 | 0x80) & 0xFF;
        cw118_disp = k3;
        cw116_disp = k2 | 0x80;
    }
}
