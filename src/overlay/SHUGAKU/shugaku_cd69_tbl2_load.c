/* @SymbolSize: 56 */
/* shugaku_cd69_tbl2_load - SHUGAKU overlay, Address: 0x801340B8, Size: 56 bytes (14 insns) */
#include "types.h"

extern void cd_read_start2(s32 a, s32 b, s32 c);
extern void shugaku_tbl_init2(void);
extern void sub_sub_step_up(void);

void shugaku_cd69_tbl2_load(void)
{
    cd_read_start2(69, 0x801B0000, 0x83FB);
    shugaku_tbl_init2();
    sub_sub_step_up();
}
