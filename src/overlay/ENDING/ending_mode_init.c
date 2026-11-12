/* @SymbolStart: 0x80133908 */
/* @SymbolSize: 200 */
/* ending_mode_init - ENDING overlay, Address: 0x80133910, Size: 192 bytes (48 insns) */
#include "types.h"

extern s32 sub_step_up();
extern void strcpy(s32, s32);
extern void ending_ptr_init();
extern s32 sub_sub_step_up();

extern s32 girl14_flag;
extern u8  now_girl;
extern u8  ending_save_girl;
extern u8  girl_no;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 g_name;
extern s32 ending_mode_init_str0;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern s32 ending_serifu_tbl[];
extern s32 ending_serifu_kind_tbl[];
extern s32 ending_sound_tbl[];

s32 ending_mode_init(void)
{
    s32 t6;
    s32 t7;
    s32 t8;
    u8  t9;
    u8  t0;
    u8  t1;

    t6 = girl14_flag;
    t7 = t6 << 20;
    t8 = (u32)t7 >> 29;
    if ((u32)t8 < 3) {
        sub_step_up();
        return 0;
    }

    t9 = now_girl;
    ending_save_girl = t9;
    t0 = 12;
    now_girl = t0;
    t1 = 12;
    girl_no = t1;
    now_serifu = 0;
    now_serifu_in = 0;
    strcpy((s32)&g_name, (s32)&ending_mode_init_str0);
    ending_ptr_init();

    ss_serifu = ending_serifu_tbl[15];
    ss_serifu_kind = ending_serifu_kind_tbl[15];
    sss_sound = ending_sound_tbl[15];
    return sub_sub_step_up();
}
