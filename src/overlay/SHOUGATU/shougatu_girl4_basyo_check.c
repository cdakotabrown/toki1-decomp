/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=2 */
/* @SymbolStart: 0x80134204 */
/* @SymbolSize: 168 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t8:t9 0x48:rs:t8:t9 0x50:rt:v0:v1 0x54:rs:v0:v1 0x54:rt:v0:v1 0x58:rt:v1:a0 0x5c:rt:v0:v1 0x60:rs:v0:a0 0x60:rt:v1:v0 0x64:rt:t9:t0 0x68:rs:t9:t0 0x68:rt:t9:t0 0x70:rs:t9:t0 0x74:rt:t0:t1 0x80:rt:t0:t1 0x84:rs:v0:a0 0x84:rt:v1:v0 0x88:rt:t1:t2 0x8c:rs:t1:t2 0x8c:rt:t1:t2 0x90:rt:t2:t3 0x94:rs:t1:t2 0x9c:rt:t2:t3 */
#include "types.h"

extern s32 get_g_zyotai_s(s32);
extern void shougatu_serifu_run(void);

extern u8 now_girl;
extern u8 girl4_basyo;
extern s16 shougatu_meet_now_serifu_in;
extern u8 shougatu_alone_flag;

void shougatu_girl4_basyo_check(void)
{
    s32 t9;
    u32 v0;

    if (shougatu_alone_flag != 0) {
        goto exit;
    }
    t9 = get_g_zyotai_s(now_girl) & 0x7F;
    if (now_girl != 4) {
        goto exit;
    }
    if ((u32)t9 < 2) {
        goto exit;
    }
    v0 = (u32)girl4_basyo >> 4;
    if (6 == v0 && shougatu_meet_now_serifu_in == 1) {
        shougatu_meet_now_serifu_in = 2;
        goto exit;
    }
    if (6 != v0 && shougatu_meet_now_serifu_in == 0) {
        shougatu_meet_now_serifu_in = 1;
    }
exit:
    shougatu_serifu_run();
}
