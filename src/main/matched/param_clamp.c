/* Address: 0x80084F44 */
/* Size: 272 bytes (68 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 screen_w[];

void param_clamp(void) {
    s16 i;

    for (i = 0; i < 11; i++) {
        u8 *rec = &screen_w[i * 56];

        if (*(s16 *)(rec + 446) < 0) {
            *(s32 *)(rec + 444) = 0;
        }
        if (*(s16 *)(rec + 446) >= 128) {
            *(s16 *)(rec + 446) = 128;
        }
        if (*(s16 *)(rec + 450) < 0) {
            *(s32 *)(rec + 448) = 0;
        }
        if (*(s16 *)(rec + 450) >= 128) {
            *(s16 *)(rec + 450) = 128;
        }
        if (*(s16 *)(rec + 454) < 0) {
            *(s32 *)(rec + 452) = 0;
        }
        if (*(s16 *)(rec + 454) >= 255) {
            *(s16 *)(rec + 454) = 255;
        }
    }
    for (i = 0; i < 9; i++) {
        u8 *rec = &screen_w[i * 4];

        if (*(s16 *)(rec + 254) < 0) {
            *(s32 *)(rec + 252) = 0;
        }
        if (*(s16 *)(rec + 254) >= 999) {
            *(s32 *)(rec + 252) = 0x03E70000;
        }
    }
}
