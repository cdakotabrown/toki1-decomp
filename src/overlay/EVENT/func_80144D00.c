/* Address: 0x80144D00 */
/* Size: 212 bytes (53 instructions) */
#include "types.h"

extern u8 D_8008093C;
extern s32 ev_flag14;
extern u8 D_800EAFA7;
extern u8 D_800EAFEB;
extern u8 D_80123988;
extern u8 D_800E9620[];
extern s32 D_8012398C;

extern void func_8004C808();
extern void func_8004C060();
extern void func_80011DFC();

void func_80144D00(void) {
    s16 i;

    D_8008093C = 10;
    func_8004C808(1);
    ev_flag14 = 1;
    D_800EAFA7 = 128;
    D_800EAFEB = 128;
    for (i = 100; i < 106; i++) {
        func_8004C060(i, 128);
    }
    D_80123988 = 0;
    for (i = 0; i < 10; i++) {
        D_800E9620[i * 68 + 6735] |= 0x80;
    }
    D_8012398C = 1;
    func_80011DFC();
}
