/* @SymbolStart: 0x80136108 */
/* @SymbolSize: 68 */
/* kangei_serifu_run2 - KANGEI overlay, Address: 0x80136110, Size: 60 bytes (15 insns) */
#include "types.h"

extern u8 kangei_serifu_mode;
extern void func_80133F08(void);
extern s16 kangei_now_s_in;

void kangei_serifu_run2(void)
{
    s32 hiddenT6 = kangei_now_s_in;

    if (hiddenT6 == 0) {
        kangei_serifu_mode = 2;
    } else {
        kangei_serifu_mode = 0;
    }
    func_80133F08();
}
