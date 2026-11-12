/* Address: 0x80140990 */
/* Size: 68 bytes (17 insns) */
/* @SymbolSize: 68 */
#include "types.h"

extern s32 taiiku_kari_buf;
extern s32 taiiku_kari_buf_1;
extern s32 taiiku_kari_buf_2;
extern s32 taiiku_kari_buf_3;

void taiiku_bufptr_init(void) {
    taiiku_kari_buf = 0x8019D000;
    taiiku_kari_buf_1 = 0x8018F000;
    taiiku_kari_buf_2 = 0x8019E198;
    taiiku_kari_buf_3 = 0x8019D008;
}
