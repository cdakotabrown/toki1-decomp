/* Address: 0x8007C7D0 */
/* Size: 68 bytes (17 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u32 bustup_check_init_tbl0;
extern u32 bustup_check_init_tbl1;
extern u32 bustup_check_init_tbl2;
extern u32 bustup_check_init_tbl3;

void bustup_ptr_init(void) {
    bustup_check_init_tbl0 = 0x80180084;
    bustup_check_init_tbl1 = 0x80180098;
    bustup_check_init_tbl2 = 0x8018009C;
    bustup_check_init_tbl3 = 0x801800AC;
}
