/* Address: 0x80141D80 */
/* Size: 68 bytes (17 insns) */
/* @SymbolSize: 68 */
#include "types.h"

extern s32 taiiku_tama_buf;
extern s32 taiiku_tama_buf_1;
extern s32 taiiku_tama_buf_2;
extern s32 taiiku_tama_buf_3;

void taiiku_bufptr_init2(void) {
    taiiku_tama_buf = 0x801A1000;
    taiiku_tama_buf_1 = 0x8018F000;
    taiiku_tama_buf_2 = 0x801A2198;
    taiiku_tama_buf_3 = 0x801A1008;
}
