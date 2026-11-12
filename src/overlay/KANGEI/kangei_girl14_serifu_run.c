/* @SymbolStart: 0x80134EB8 */
/* @SymbolSize: 88 */
#include "types.h"

extern void func_80133F08(void);
extern u8 girl_no;
extern u8 now_girl[];

void kangei_girl14_serifu_run(void)
{
    s32 pad;
    s32 old;

    now_girl[0] = 14;
    old = now_girl[0x1AB];
    func_80133F08();
    if (old != now_girl[0x1AB]) {
        now_girl[0] = girl_no;
    }
}
