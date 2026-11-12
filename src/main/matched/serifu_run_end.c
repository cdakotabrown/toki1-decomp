/* Address: 0x80085C44 (recorded) / real entry 0x80085C00 */
/* Size: 52 bytes (recorded) / 120 bytes over the absorbed head */
/* Region: Game (0x80041000-0x80086000) */
/* Compiler: IDO 5.2 */
/* @SymbolStart: 0x80085C00 */
/* @SymbolSize: 120 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8   now_serifu[];
extern u8   now_serifu_in[];
extern s32  ss_serifu;
extern s32  ss_serifu_kind;
extern s32  sss_sound;
extern u8  *now_s;
extern u8  *now_s_in;
extern s32  ss_s;
extern s32  ss_s_kind;
extern s32  sss_so;

extern void serifu_main(s32 a, s32 b, s32 c);

void serifu_run_end(void)
{
    now_s = now_serifu;
    now_s_in = now_serifu_in;
    ss_s = ss_serifu;
    ss_s_kind = ss_serifu_kind;
    sss_so = sss_sound;
    serifu_main(255, 1, 0);
}
