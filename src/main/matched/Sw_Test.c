/* Address: 0x80053DF0 */
/* Size: 200 bytes (50 instructions) */
/* Region: Game */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8  card_result;
extern s32 CE0;
extern s32 CE1;
extern s32 CE2;
extern s32 CE3;
extern s32 TestEvent(s32 arg0);
extern void Sw_Clear(void);

u32 Sw_Test(void) {
    card_result = 0;
    if (TestEvent(CE0) != 0) {
        card_result = 1;
    } else if (TestEvent(CE1) != 0) {
        card_result = 2;
    } else if (TestEvent(CE2) != 0) {
        card_result = 4;
    } else if (TestEvent(CE3) != 0) {
        card_result = 8;
    }
    if (card_result != 0) {
        Sw_Clear();
    }
    return (u32)card_result;
}
