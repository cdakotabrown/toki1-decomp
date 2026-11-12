/* @CompileEnv: IDO52_UGEN_NO_ARG_COALESCE=1 */
/* omimai_girl_serifu_init - OMIMAI overlay, Address: 0x80133804, Size: 136 bytes (34 insns) */
/* @SymbolStart: 0x801337EC */
/* @SymbolSize: 160 */
#include "types.h"

extern u8 schedule_cursor;
extern u8 schedule_kind[];
extern u8 now_girl;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern s32 omimai_serifu3_res_tbl[][13];

extern void date_name_set(s32);
extern void omimai_serifu3_tbl_init(void);

void omimai_girl_serifu_init(void)
{
    s32 a0;
    u8 t9;

    a0 = schedule_kind[schedule_cursor * 4];
    now_girl = a0;
    date_name_set(a0);
    omimai_serifu3_tbl_init();
    t9 = now_girl;
    if (t9 == 12) {
        now_serifu = 8;
    } else {
        now_serifu = 0;
    }
    now_serifu_in = 0;
    ss_serifu = omimai_serifu3_res_tbl[0][11];
    ss_serifu_kind = omimai_serifu3_res_tbl[1][11];
    sss_sound = omimai_serifu3_res_tbl[2][11];
}
