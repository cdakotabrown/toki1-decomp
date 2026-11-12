/* Function: serifu_run_step */
/* Address: 0x80085BB4 */
/* Size: 136 bytes (34 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x80085B70 */
/* @SymbolSize: 144 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 now_s;
extern s32 now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;
extern s32 now_serifu;
extern s32 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern u8  serifu_no;

extern s32 serifu_main(u8 arg0, s32 arg1, s32 arg2);
extern void sub_sub_step_up(void);

void serifu_run_step(void) {
    now_s = (s32)&now_serifu;
    now_s_in = (s32)&now_serifu_in;
    ss_s = ss_serifu;
    ss_s_kind = ss_serifu_kind;
    sss_so = sss_sound;
    if (serifu_main(serifu_no, 1, 0) == 1) {
        sub_sub_step_up();
    }
}
