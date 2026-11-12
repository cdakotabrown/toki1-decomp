/* @SymbolSize: 172 */
/* @CompileFlags: -cckr */
#include "types.h"

extern s32 step_counter;
extern s32 **shougatu_meet_ss_serifu;
extern u8 **shougatu_meet_ss_serifu_kind;
extern s16 shougatu_meet_now_serifu;
extern s16 shougatu_meet_now_serifu_in;
extern s32 shougatu_serifu_sound_check_str0;

shougatu_serifu_sound_check()
{
    shougatu_serifu_run();
    if (step_counter++ == 0) {
        if (shougatu_meet_ss_serifu_kind[shougatu_meet_now_serifu][shougatu_meet_now_serifu_in - 1] == 7) {
            if (strcmp(shougatu_meet_ss_serifu[shougatu_meet_now_serifu][shougatu_meet_now_serifu_in - 1], &shougatu_serifu_sound_check_str0) == 0) {
                device_cmd_send(1548);
            }
        }
    }
}
