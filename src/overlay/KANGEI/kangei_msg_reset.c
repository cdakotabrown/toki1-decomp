/* Address: 0x80138F5C */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 kangei_bank2_tbl_0_1;
extern s32 kangei_bank2_tbl_1_1;
extern s32 kangei_bank2_tbl_2_1;
extern s32 kangei_ss_s;
extern s32 kangei_ss_s_kind;
extern s32 kangei_sss_so;

void kangei_msg_reset(void) {
    kangei_ss_s = kangei_bank2_tbl_0_1;
    kangei_ss_s_kind = kangei_bank2_tbl_1_1;
    kangei_sss_so = kangei_bank2_tbl_2_1;
}
