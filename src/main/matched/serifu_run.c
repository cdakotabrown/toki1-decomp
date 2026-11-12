/* Address: 0x80085C78 */
/* Size: 124 bytes (31 instructions) */
/* Region: IDO (>= 0x80081000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 now_s;
extern s32 now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern u8  serifu_no;
extern u8  now_serifu[];
extern u8  now_serifu_in[];

extern void serifu_main(u8 arg0, s32 arg1, s32 arg2);

void serifu_run(void) {
    now_s = (s32)now_serifu;
    now_s_in = (s32)now_serifu_in;
    ss_s = ss_serifu;
    ss_s_kind = ss_serifu_kind;
    sss_so = sss_sound;
    serifu_main(serifu_no, 1, 0);
}
