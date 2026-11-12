/* @SymbolStart: 0x8013BC7C */
/* @SymbolSize: 212 */
/* @PostProc: patch_insn_reg_field 0x50:rt:v1:v0 0x5c:rs:v1:v0 0x5c:rt:v1:v0 0x64:rs:v1:v0 0x68:rs:v1:v0 */
/* Address: 0x8013BC88, Size: 200 bytes (50 instructions) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 serifu_main(s32 a0, s32 a1, s32 a2, s32 a3);

extern s32 now_s;
extern s32 now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;
extern s32 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern u8 date_pattern;
extern u16 basyo_pack;
extern u8 now_girl;
extern u8 serifu_no;

void date_basyo13_serifu_run(s32 a0, s32 a1, s32 a2, s32 a3)
{
    s32 guard;
    s32 cnt;

    if (a0) { }
    if (a1) { }
    if (a2) { }

    guard = now_girl;
    now_s = (s32)&now_serifu;
    if (guard == 0) {
        if (((u32)basyo_pack >> 12) == 6) {
            if (date_pattern == 0) {
                cnt = now_serifu_in;
                if (cnt == 0) {
                    now_serifu_in = cnt + 1;
                }
            }
        }
    }
    now_s_in = (s32)&now_serifu_in;
    ss_s = ss_serifu;
    ss_s_kind = ss_serifu_kind;
    sss_so = sss_sound;
    if (serifu_main(serifu_no, 0, 0, a3) != 0) {
        sub_sub_step_up();
    }
}
