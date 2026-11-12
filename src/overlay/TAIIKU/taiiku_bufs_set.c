/* Size: 68 bytes */
/* @SymbolSize: 68 */
#include "types.h"

extern s32 taiiku_spoon_buf;
extern s32 taiiku_spoon_buf_1;
extern s32 taiiku_spoon_buf_2;
extern s32 taiiku_spoon_buf_3;

void taiiku_bufs_set(void) {
    taiiku_spoon_buf = 0x801A19CC;
    taiiku_spoon_buf_1 = 0x8018F000;
    taiiku_spoon_buf_2 = 0x8019D000;
    taiiku_spoon_buf_3 = 0x8019E190;
}
