/* @SymbolSize: 104 */
/* en_nichi_play_main - EN_NICHI overlay, Address: 0x80132B88, Size: 104 bytes (26 insns) */
#include "types.h"

extern void en_nichi_konami_command_set();
extern void en_nichi_fade_in();
extern void en_nichi_sway();
extern void en_nichi_phase_count();
extern void en_nichi_action_main();
extern void en_nichi_obj();

extern s32 en_nichi_cmd_wait_cnt;

void en_nichi_play_main(void)
{
    s32 v0;

    en_nichi_konami_command_set();
    v0 = en_nichi_cmd_wait_cnt - 1;
    en_nichi_cmd_wait_cnt = v0;
    if (v0 < 0) {
        en_nichi_cmd_wait_cnt = 0;
    }
    en_nichi_fade_in();
    en_nichi_sway();
    en_nichi_phase_count();
    en_nichi_action_main();
    en_nichi_obj();
}
