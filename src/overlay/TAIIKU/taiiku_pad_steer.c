/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=1 IDO52_FORCE_WEB_COLOR=12:a0,51:v0,56:v1 IDO52_UGEN_TEMP_REG=8:a0,10:a0 IDO52_UGEN_TEMP_REG_INPLACE=8,10 IDO52_AS1_TIE_SRC_ORDER=bb:4 */
/* @SymbolStart: 0x80147AC4 */
/* @SymbolSize: 324 */
/* @CompileFlags: -cckr */
#include "types.h"

extern int taiiku_spoon_pl_spd;
extern int taiiku_spoon_scrv;
extern unsigned short taiiku_spoon_bal_0_wobble;
extern volatile int taiiku_spoon_bal_0_tilt;
extern int taiiku_spoon_vmax;
extern int pad_data;

void taiiku_pad_steer()
{
    int r;
    int v;
    unsigned int a;
    int x;
    int up;
    int dn;

    if (taiiku_spoon_pl_spd > 0 || taiiku_spoon_scrv != 0) {
        if (taiiku_spoon_bal_0_wobble >= 101 && (a = taiiku_spoon_bal_0_tilt) == 0) {
            r = rand();
            if (taiiku_spoon_pl_spd == taiiku_spoon_vmax) {
                v = 15;
            } else {
                v = 0;
            }
            a = (unsigned short)(r % 100);
            if (a < v + 25) {
                taiiku_spoon_bal_0_wobble = 0;
                a = -100;
                taiiku_spoon_bal_0_tilt = a;
            } else if (a < v + 50) {
                taiiku_spoon_bal_0_wobble = 0;
                a = 100;
                taiiku_spoon_bal_0_tilt = a;
            }
        }
        x = pad_data;
        a = taiiku_spoon_bal_0_tilt;
        up = x & 0xA;
        dn = x & 5;
        if (up && !(x & 5)) {
            a -= 11;
            if (a == 0) {
                taiiku_spoon_bal_0_wobble = 0;
            }
        }
        taiiku_spoon_bal_0_tilt = a;
        if (dn) {
            taiiku_spoon_bal_0_tilt = a;
            if (!up) {
                a += 11;
                taiiku_spoon_bal_0_tilt = a;
                if (a == 0) {
                    taiiku_spoon_bal_0_wobble = 0;
                }
            }
        }
    }
}
