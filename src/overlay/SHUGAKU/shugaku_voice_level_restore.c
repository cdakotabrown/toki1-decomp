/* @SymbolStart: 0x80135228 */
/* @SymbolSize: 56 */
/* shugaku_voice_level_restore - SHUGAKU overlay, Address: 0x80135234, Size: 44 bytes (11 insns) */
#include "types.h"

extern u8  voice_level;
extern u8  shugaku_voice_save;
extern s16 now_serifu_shugaku;
extern void sub_sub_step_up(void);

void shugaku_voice_level_restore(void)
{
    voice_level = shugaku_voice_save;
    now_serifu_shugaku = 41;
    sub_sub_step_up();
}
