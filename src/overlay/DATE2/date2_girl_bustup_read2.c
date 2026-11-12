/* @SymbolStart: 0x80133614 */
/* @RodataBase: 0x8013A128 */
/* Address: 0x8013361C, Size: 256 bytes (64 insns) */
/* @SymbolSize: 256 */
#include "types.h"

extern void date2_girl_res_set(void);
extern void date_name_set(s32 arg0);
extern void bustup_cd_read(s32 arg0);
extern void sub_sub_step_up(void);

extern u8 now_girl;
extern u8 girl_no;

void date2_girl_bustup_read2(void) {
    s32 val;

    now_girl = girl_no;
    date2_girl_res_set();
    date_name_set((s32)now_girl);
    val = (s32)now_girl - 1;
    switch (val) {
    case 0: bustup_cd_read(24008); break;
    case 1: bustup_cd_read(21173); break;
    case 2: bustup_cd_read(22838); break;
    case 3: bustup_cd_read(26438); break;
    case 4: bustup_cd_read(24638); break;
    case 5: bustup_cd_read(25268); break;
    case 6: bustup_cd_read(23423); break;
    case 7: bustup_cd_read(20588); break;
    case 8: bustup_cd_read(25898); break;
    }
    sub_sub_step_up();
}
