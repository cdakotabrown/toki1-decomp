/* @CompileFlags: -Wo,-loopunroll,0 */
/* @SymbolSize: 196 */
/* date_trans_set - DATE overlay, Address: 0x80136F98, Size: 196 bytes (49 insns) */
#include "types.h"

extern s16 kao2[];
extern s16 imp[];
extern s16 ans[];
extern void sub_sub_step_up(void);

void date_trans_set(void)
{
    s16 i;

    for (i = 0; i < 3; i++) {
        kao2[i] = imp[i];
        if (kao2[i] == 0) {
            kao2[i] = 1;
        }
        if (kao2[i] == 4) {
            kao2[i] = 3;
        }
        ans[i] = imp[i] - 1;
        if (ans[i] == -1) {
            ans[i] = 0;
        }
        if (ans[i] == 3) {
            ans[i] = 2;
        }
    }
    sub_sub_step_up();
}
