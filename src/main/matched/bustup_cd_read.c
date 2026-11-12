/* Likely Symbol: bustup_cd_read (from o.bin, offset +64 bytes from 0x80062EB0) */
/* Address: 0x80062E70 - 0x80062EAC (60 bytes) */
/* Debug Symbol: bustup_cd_read (from o.bin, high match) */
/* Region: GCC (OR encoding) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern u8 bustup2_cw_disp;
extern u8 bustup_cw_disp;
extern u8 bustup_cd_read_tbl0;

void bustup_cd_read(s32 a0) {
    cd_read_start2(45, 0x80180000, a0);
    bustup2_cw_disp = 0;
    bustup_cw_disp = 0;
    bustup_cd_read_tbl0 = 0;
}
