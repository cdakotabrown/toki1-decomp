/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=0 */
/* @CompileFlags: -cckr */
#include "types.h"

struct Names {
    char n[22][33];
};

extern struct Names D_80125504;
extern unsigned char D_800B0F2C;
extern unsigned char D_800B0F2D;
extern unsigned char D_800B0F2E;
extern unsigned char D_800B0F2F;
extern unsigned char D_800B1AF6;
extern unsigned char D_801252E4;
extern short D_80124384;
extern char D_80094818[];
extern char D_8009483C[];

void func_80156C18()
{
    int unused;
    struct Names buf;

    buf = D_80125504;
    if ((D_800B0F2C == D_800B0F2D && D_801252E4 == 1) || D_800B0F2E == 255) {
        D_800B1AF6 += 17;
        return;
    }
    if (D_800B0F2C == D_800B0F2E) {
        func_80078970(D_80094818, buf.n[D_800B0F2D]);
    } else {
        func_80078970(D_80094818, buf.n[D_800B0F2E]);
    }
    if (D_800B0F2F != 255) {
        D_80124384 = 3;
        if (D_800B0F2C == D_800B0F2F) {
            func_80078970(D_8009483C, buf.n[D_800B0F2D]);
        } else {
            func_80078970(D_8009483C, buf.n[D_800B0F2F]);
        }
    }
    func_80011DFC();
}
