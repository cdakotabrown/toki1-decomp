/* @SymbolStart: 0x80158218 */
/* @SymbolSize: 48 */
/* date_girl9_serifu_girl9 - DATE overlay, Address: 0x80158220 */
#include "types.h"

extern u8 now_girl;
extern s32 kuchi_stop;
extern void func_8014C530(void);

void date_girl9_serifu_girl9(void)
{
    now_girl = 9;
    kuchi_stop = 0;
    func_8014C530();
}
