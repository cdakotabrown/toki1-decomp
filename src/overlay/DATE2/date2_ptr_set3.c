/* Size: 68 bytes */
/* @SymbolSize: 68 */
#include "types.h"
extern u8 *date2_sched2_str_tbl;
extern u8 *date2_sched0_str_tbl;
extern u8 *date2_sched3_str_tbl;
extern u8 *date2_sched1_str_tbl;
void date2_ptr_set3(void)
{
    date2_sched2_str_tbl = (u8 *)0x801E80D0;
    date2_sched0_str_tbl = (u8 *)0x801E82C0;
    date2_sched3_str_tbl = (u8 *)0x801E8444;
    date2_sched1_str_tbl = (u8 *)0x801E8700;
}
