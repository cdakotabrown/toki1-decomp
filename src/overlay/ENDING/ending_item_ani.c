/* @CompileFlags: -Wo,-loopunroll,0 */
/* @SymbolSize: 388 */
/* ending_item_ani - ENDING overlay, Address: 0x80136D80, Size: 388 bytes (97 insns; audit 396 includes the next symbol's 2-word head) */
#include "types.h"

extern s32 rand(void);

extern s16 ending_item_blink[];
extern u8 class_w[][68];

void ending_item_ani(void)
{
    s16 i;
    s32 t;
    s32 m80;
    s32 m1;
    s32 k60;
    s32 v;

    m80 = -129;
    m1 = -2;
    k60 = 60;
    for (i = 0; i < 14; i++) {
        t = ending_item_blink[i];
        if (t != 0) {
            ending_item_blink[i] = t - 1;
            class_w[i][6531] = class_w[i][6531] & m80;
            class_w[i][6530] = class_w[i][6530] & m1;
        } else {
            class_w[i][6531] |= 0x80;
            class_w[i][6530] |= 1;
            if (*(s16 *)&class_w[i][6552] == 0) {
                v = *(s16 *)&class_w[i][6536];
                if (v == 0) {
                    class_w[i][6531] = class_w[i][6531] & m80;
                    class_w[i][6530] = class_w[i][6530] & m1;
                    *(s16 *)&class_w[i][6536] = v + 1;
                    ending_item_blink[i] = rand() % k60 + 60;
                    *(s16 *)&class_w[i][6550] = rand() % 5;
                }
            }
        }
    }
}
