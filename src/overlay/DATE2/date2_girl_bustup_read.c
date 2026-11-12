/* @SymbolStart: 0x801335D8 */
/* @SymbolSize: 60 */
/* date2_girl_bustup_read - DATE2 overlay, Address: 0x801335E0, Size: 52 bytes (13 insns) */
#include "types.h"

extern u8 now_girl;
extern u8 girl_no;

void date_name_set(s32 arg0);
void date2_girl_res_set(void);
void girl_season_load(void);

void date2_girl_bustup_read(void)
{
    s32 a0;

    a0 = girl_no;
    now_girl = (u8)a0;
    date_name_set(a0);
    date2_girl_res_set();
    girl_season_load();
}
