/* @SymbolStart: 0x80139E40 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=4 IDO52_FORCE_WEB_COLOR=4:v1 */
/* @PostProc: swap_branch_operands --offsets 0x5c,0xa0,0xb0,0x108 */
/* @SymbolSize: 364 */
/* date_score_add -- DATE overlay, Address: 0x80139E40 (head) / 0x80139E48, */
/* Size: 364 bytes (91 insns) */
#include "types.h"

extern void date_trans_set(void);

extern u8 now_girl;
extern s16 imp;
extern s16 imp_1;
extern s16 imp_2;
extern s16 now_serifu;

void date_score_add(void)
{
    s16 v0;
    s16 t0, t1;

    switch (now_girl) {
    case 4:

        v0 = imp;
        if (v0 == 3) {
            now_serifu = now_serifu + 1;
        } else if (v0 == 2) {
            now_serifu = now_serifu + 2;
        }
        break;

    case 9:

        v0 = imp;
        if (v0 == 3) {
            t0 = imp_1;
            if (t0 == 3) {
                now_serifu = now_serifu + 2;
            } else {
                now_serifu = now_serifu + 1;
            }
        }
        break;

    case 5:
    case 6:

        v0 = imp;
        if (v0 == 4) {
            now_serifu = now_serifu + 1;
        } else if (v0 == 3) {
            now_serifu = now_serifu + 2;
        }
        break;

    default:

        v0 = imp;
        if (v0 == 4) {
            now_serifu = now_serifu + 1;
        } else if (v0 == 2) {
            now_serifu = now_serifu + 2;
        }
        break;
    }

    date_trans_set();
}
