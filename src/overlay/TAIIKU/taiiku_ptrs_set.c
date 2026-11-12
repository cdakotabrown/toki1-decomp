/* @SymbolSize: 52 */
/* Address: 0x80132b90 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 taiiku_main_serifu;
extern s32 taiiku_main_serifu_1;
extern s32 taiiku_main_serifu_2;

void taiiku_ptrs_set(void) {
    taiiku_main_serifu = 0x801F0164;
    taiiku_main_serifu_1 = 0x801F019C;
    taiiku_main_serifu_2 = 0x801F027C;
}
