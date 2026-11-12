/* Address: 0x80132e50 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern volatile s32 kangei_girl0_res_tbl;
extern volatile s32 kangei_girl0_res_tbl_1;
extern volatile s32 kangei_girl0_res_tbl_2;

void kangei_girl0_desc_set(void) {
    kangei_girl0_res_tbl = 0x801C1400;
    kangei_girl0_res_tbl_1 = 0x801C1D6C;
    kangei_girl0_res_tbl_2 = 0x801CA47C;
}
