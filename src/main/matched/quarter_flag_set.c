/* Function: quarter_flag_set */
/* Address: 0x80072384 */
/* Size: 116 bytes (29 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @SymbolStart: 0x80072384 */
/* @SymbolSize: 116 */
/* @CompileFlags: -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8 pad[0xF64];
    u32 lo : 13;
    u32 f13 : 1;
    u32 f14 : 1;
    u32 hi : 17;
} GameState;

extern u8 now_month;
extern GameState screen_w;

quarter_flag_set()
{
    GameState *p;

    switch (now_month) {
    case 2:
        break;
    case 3:
    case 6:
    case 9:
    case 12:
        p = &screen_w;
        p->f14 = screen_w.f13;
        p->f13 = 0;
        break;
    }
}
