/* Address: 0x800412D8 */
/* Size: 76 bytes (19 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @Status: SEMI-MATCHED — DEMOTED 2026-08-13 (custom_script fabrication audit) */
/* `@PostProc: custom_script custom_func_800412D8.py`, now retired and deleted. */
/* @Measured: 2026-09-24 0/19 exact (0.0%), compiled 23 vs target 19 words -- SEMI (overruns window) */
#include "types.h"

extern void ClearImage(RECT *r, s32 a1, s32 a2, s32 a3);
extern void DrawSync(s32 arg0);

void func_800412D8(s32 a0, s32 a1, s32 a2, s32 a3) {
    RECT rect;

    rect.w = 640;
    rect.h = 480;
    rect.x = 0;
    rect.y = 0;
    ClearImage(&rect, 0, 0, 0);
    DrawSync(0);
}
