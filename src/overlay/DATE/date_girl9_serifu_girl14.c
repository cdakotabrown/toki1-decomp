/* @SymbolStart: 0x80158248 */
/* @SymbolSize: 72 */
/* Function: date_girl9_serifu_girl14 */
/* Overlay: DATE */
#include "types.h"

extern void func_8014C530(void);

extern u16 now_serifuev_in;
extern u8  now_girl;
extern s32 bustup_mode;

void date_girl9_serifu_girl14(void)
{
    if (now_serifuev_in >= 4U) {
        now_girl = 14;
        bustup_mode = 2;
    }
    func_8014C530();
}
