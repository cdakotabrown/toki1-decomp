/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=0:v1 */
/* @SymbolStart: 0x80133760 */
/* @SymbolSize: 280 */
/* Address: 0x80133760 (absorbed head) .. 0x80133878, Size: 280 bytes */
#include "types.h"

extern void sub_sub_step_up();
extern void date_name_set();
extern void shougatu_girl_serifu_set();
extern void func_80133980();
extern u8 now_girl;
extern u8 shougatu_girl_found;
extern s32 shougatu_meet_ss_serifu;
extern s32 shougatu_meet_ss_serifu_kind;
extern s32 shougatu_meet_sss_sound;
extern u16 shougatu_meet_now_serifu;

typedef struct {
    unsigned p0:3; unsigned b3:1; unsigned p1:28;
    u8 pad[52];
} Rec56;
extern Rec56 girl0_flag[];

extern s32 serifu_no;
extern s32 shougatu_meet_serifu_tbl[];
extern s32 shougatu_meet_serifu_kind_tbl[];
extern s32 shougatu_meet_sound_tbl[];

shougatu_girl_taku_set()
{
    int n;

    if (serifu_no == 11) {
        func_80133980();
        return 0;
    }
    n = serifu_no;
    if (now_girl == n && shougatu_girl_found != 0) {
        sub_sub_step_up();
        return 0;
    }
    if (girl0_flag[n].b3 == 0) {
        shougatu_meet_now_serifu = 0;
        shougatu_meet_ss_serifu = shougatu_meet_serifu_tbl[14];
        shougatu_meet_ss_serifu_kind = shougatu_meet_serifu_kind_tbl[14];
        shougatu_meet_sss_sound = shougatu_meet_sound_tbl[14];
        date_name_set(n & 0xFF);
        sub_sub_step_up();
        return 0;
    }
    now_girl = n;
    shougatu_girl_serifu_set();
    date_name_set(now_girl);
    shougatu_meet_now_serifu = 9;
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
}
