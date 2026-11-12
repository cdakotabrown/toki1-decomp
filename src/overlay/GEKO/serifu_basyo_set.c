/* @CompileEnv: IDO52_SKIP_PASSES=eliminduct */
/* `@CompileEnv: IDO52_SKIP_PASSES=eliminduct` removes that inserted copy on */
/* @SymbolStart: 0x801339AC */
/* @SymbolSize: 244 */
/* @PostProc: swap_branch_operands --offsets 0xa0 */
/* serifu_basyo_set -- GEKO overlay, Address: 0x801339B4 (true start 0x801339AC) */
/* Size: 244 bytes (61 insns) including the absorbed 2-word head. */
#include "types.h"

extern void sub_sub_step_up(void);

extern u8 geko_serifu_state;

extern u16 basyo_pack;
extern u8 girl4_basyo;
extern u8 now_girl;
extern s16 geko_now_serifu;

void serifu_basyo_set(void)
{
    u32 v0;
    u32 h;

    if (geko_serifu_state == 100) {
        sub_sub_step_up();
        sub_sub_step_up();
        sub_sub_step_up();
    } else if (now_girl == 4) {
        if (geko_now_serifu == 38) {
            v0 = (u32)basyo_pack >> 12;
            if (v0 != 6U) goto seven;
            if (((u32)girl4_basyo >> 4) != 6U) goto seven;
            geko_now_serifu = 40;
            goto tail;
seven:
            if (v0 != 7U) goto fortytwo;
            if (((u32)girl4_basyo >> 4) != 7U) goto fortytwo;
            geko_now_serifu = 41;
            goto tail;
fortytwo:
            geko_now_serifu = 42;
        } else {
            sub_sub_step_up();
        }
    } else {
        sub_sub_step_up();
    }

tail:
    sub_sub_step_up();
}
