/* @SymbolSize: 68 */
/* Address: 0x80133c30 */
/* Size: 68 bytes (17 insns) */
#include "types.h"

extern s32 taiiku_com_buf;
extern s32 taiiku_com_buf_1;
extern s32 taiiku_com_buf_2;
extern s32 taiiku_com_buf_3;

void taiiku_ptr4_init(void) {
    taiiku_com_buf = 0x801C3000;
    taiiku_com_buf_1 = 0x801C300C;
    taiiku_com_buf_2 = 0x801C479C;
    taiiku_com_buf_3 = 0x801B3000;
}
