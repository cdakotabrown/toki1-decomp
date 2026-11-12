/* @CompileEnv: IDO52_COPYPROP_VAR=-8:2-3 */
/* @SymbolSize: 328 */
/* valen_list_shuffle - VALEN overlay, Address: 0x801337CC, Size: 328 bytes (82 insns) */
#include "types.h"

extern s32 frame_counter;
extern s16 valen_girl_list_cnt;
extern s16 valen_girl_list[];

extern void srand(s32 a0);
extern s32 rand(void);

void valen_list_shuffle(void)
{
    s32 val;
    s32 n;
    s16 count;
    s16 i;
    s16 a;
    s16 b;
    s16 tmp;

    srand(frame_counter);
    val = rand();
    n = val & 0xFF;
    if (val < 0 && n != 0) {
        n -= 256;
    }
    n += 10;
    count = (s16)n;
    for (i = 0; i < count; i++) {
        a = (s16)(rand() % valen_girl_list_cnt);
        b = (s16)(rand() % valen_girl_list_cnt);
        tmp = valen_girl_list[a];
        valen_girl_list[a] = valen_girl_list[b];
        valen_girl_list[b] = tmp;
    }
}
