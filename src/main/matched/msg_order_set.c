/* Address: 0x800830BC */
/* Size: 188 bytes (47 instructions) -- the audit's 196 includes the hoisted */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @SymbolSize: 188 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:v1,9:v0 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 frame_counter;
u8 junban[3] = { 0, 1, 2 };

void msg_order_set(void) {
    s32 v = frame_counter;
    s32 r = ((u32)v % 3u) & 0xFF;
    s32 a;

    if (r == 0) {
        junban[1] = 1 + r + (v & 1);
        junban[2] = 1 + r + ((v + 1) & 1);
        junban[0] = r;
    } else {
        a = r - 1;
        if (r == 1) {
            junban[1] = a + (u32)((v & 1) << 1);
            junban[2] = a + (u32)(((v + 1) & 1) << 1);
            junban[0] = r;
        } else {
            junban[1] = a - (v & 1);
            junban[2] = a - ((v + 1) & 1);
            junban[0] = r;
        }
    }
}
