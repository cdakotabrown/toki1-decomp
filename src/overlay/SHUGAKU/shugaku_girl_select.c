/* @SymbolStart: 0x80139894 */
/* @SymbolSize: 268 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=1:a1,2:t6,3:t7,4:t8,5:t9,6:t0,7:t1,8:t2,9:t3,10:t4,11:a1,12:t5 IDO52_UGEN_TEMP_REG_INPLACE=1,11 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xcc */
/* @PostProc: swap_branch_operands --offsets 0x28,0x50 */
#include "types.h"

extern void func_80138664(void);
extern void date_name_set(s32);
extern void shugaku_serifu_set(void);
extern void sub_sub_step_up(void);

extern u8  now_sub_sub_step;
extern u8  now_girl;
extern s16 now_serifu_shugaku;
extern s32 ss_serifu_shugaku;
extern s32 ss_serifu_shugaku_kind;
extern s32 sss_sound_shugaku;
extern u8  date_place;
extern s32 serifu_no;
extern s32 shugaku_serifu_tbl[];
extern s32 shugaku_serifu_kind_tbl[];
extern s32 shugaku_sound_tbl[];

s32 shugaku_girl_select(void)
{
    s32 sel;
    u8 saved;
    s32 cur;

    sel = serifu_no;
    if (now_girl == sel) {
        saved = now_sub_sub_step;
        func_80138664();
        cur = now_sub_sub_step;
        if (saved != cur) {
            now_serifu_shugaku -= 2;
            now_sub_sub_step = cur - 3;
        }
        return 0;
    }
    if (sel == 11) {
        now_serifu_shugaku = 0;
        ss_serifu_shugaku = shugaku_serifu_tbl[12];
        ss_serifu_shugaku_kind = shugaku_serifu_kind_tbl[12];
        sss_sound_shugaku = shugaku_sound_tbl[12];
        date_place = 1;
        now_girl = 13;
        date_name_set(13);
        now_sub_sub_step = 51;
        return 0;
    }
    now_girl = sel;
    date_name_set(sel & 0xFF);
    shugaku_serifu_set();
    sub_sub_step_up();
}
