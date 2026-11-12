/* Address: 0x8013DB00 (true start 0x8013DAF8: the now_girl state-byte load */
/* is cracked by @CompileEnv: IDO52_CONST_INREG_VALUES=2. Also fixed the */
/* @SymbolStart: 0x8013DAF8 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=2 */
/* @SymbolSize: 240 */
/* @PostProc: patch_insn_reg_field 0x44:rs:v1:a0 0x44:rt:a0:v1 0x74:rs:v1:a0 0x74:rt:a0:v1 0xcc:rs:v1:a0 0xcc:rt:a0:v1 */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 goto_sub_sub_step(s32);
extern s32 serifu_run(s32);

extern s16 now_serifu;
extern u8 girl5_spot4_cnt;
extern u8 girl6_spot4_cnt;
extern u8 now_girl;
extern u8 now_sub_sub_step;
extern s32 serifu_no;

s32 date_basyo2_girl_serifu_step(void)
{
    s32 v0 = now_girl;
    s32 tmp;

    if (v0 == 6 && girl6_spot4_cnt != 0 && serifu_no == 2) {
        now_serifu = 0;
        goto_sub_sub_step(0);
    } else if (v0 == 8 && serifu_no == 2) {
        tmp = now_sub_sub_step;
        serifu_run(2);
        if (tmp != now_sub_sub_step) {
            now_serifu = 0;
            goto_sub_sub_step(0);
        }
    } else if (v0 == 5 && girl5_spot4_cnt == 0 && serifu_no == 2) {
        now_serifu = 3;
        serifu_run(2);
    } else {
        sub_sub_step_up();
    }

}
