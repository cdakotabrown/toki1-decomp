/* Function: Hw_Test */
/* Address: 0x80053F10 */
/* Size: 204 bytes (51 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 TestEvent(s32 arg);
extern void Hw_Clear(void);

extern s32 CEH0;
extern s32 CEH1;
extern s32 CEH2;
extern s32 CEH3;
extern u8 card_result;

u8 Hw_Test(void) {
    card_result = 0;

    do {
        if (TestEvent(CEH0)) {
            card_result = 129;
        } else if (TestEvent(CEH1)) {
            card_result = 130;
        } else if (TestEvent(CEH2)) {
            card_result = 132;
        } else if (TestEvent(CEH3)) {
            card_result = 136;
        }
    } while (card_result == 0);

    Hw_Clear();

    return card_result;
}
