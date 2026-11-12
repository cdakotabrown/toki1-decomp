/* @CompileEnv: IDO52_FORCE_WEB_SPILL=2 */
/* @SymbolStart: 0x8013894C */
/* @SymbolSize: 176 */
/* kangei_girl_basyo_check - KANGEI overlay, Address: 0x80138974 (head at 0x8013894C), */
#include "types.h"

extern void sub_sub_step_up(void);

extern u8 kangei_girl_save_no;
extern u8 now_girl;
extern u8 girl0_basyo[];
extern u16 basyo_pack;
extern s32 kangei_ss_s;
extern s32 kangei_ss_s_kind;
extern s32 kangei_sss_so;
extern s32 kangei_bank1_tbl[][13];

void kangei_girl_basyo_check(void)
{
    kangei_girl_save_no = now_girl;
    if (((u32)basyo_pack >> 12) == ((u32)girl0_basyo[now_girl * 56] >> 4)) {
        sub_sub_step_up();
        sub_sub_step_up();
    } else {
        now_girl = 14;
        kangei_ss_s = kangei_bank1_tbl[0][2];
        kangei_ss_s_kind = kangei_bank1_tbl[1][2];
        kangei_sss_so = kangei_bank1_tbl[2][2];
    }
    sub_sub_step_up();
}
