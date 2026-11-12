/* Size: 396 bytes */
/* @SymbolSize: 396 */
#include "types.h"

typedef struct {
    s32 kind;
    u8  pad4[18];
    s16 x;
    s16 unk24;
    s16 y;
    u8  pad28[20];
} Slot;

extern Slot en_nichi_obj_wk[10];

void en_nichi_obj_out_clear(void)
{
    s32 i;

    for (i = 0; i < 10; i++) {
        if (en_nichi_obj_wk[i].x < -192 || en_nichi_obj_wk[i].x >= 193 ||
            en_nichi_obj_wk[i].y < -152 || en_nichi_obj_wk[i].y >= 153) {
            en_nichi_obj_wk[i].kind = 0;
        }
    }
}
