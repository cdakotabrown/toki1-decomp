/* Address: 0x80057884 */
/* Size: 292 bytes (73 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:7 */
/* @PostProc: strip_leading_instructions --count auto */
/* 2. `@CompileEnv: IDO52_PRECOLOR_WEB=v0:7` -- with the const web out of the */
#include "types.h"

extern s32 dec_bg_handle;
extern u8  bg_change_tbl0[];
extern s32 dec_env_set(void *, void *, void *);
extern s32 decdct(s32, s32, s32, s32);

s32 dec_bg_init(s32 mode) {
    s32 v;

    dec_bg_handle = 0;
    dec_env_set((void *)0x80167000, (void *)0x80162000, (void *)0x80162000);
    if (mode == 0) {
        v = 1 - bg_change_tbl0[mode];
        switch (v) {
        case 0:
            dec_bg_handle = decdct(704, 160, 320, 88);
            v = 0;
            break;
        case 1:
            dec_bg_handle = decdct(320, 416, 320, 88);
            v = 1;
            break;
        }
        return v;
    } else if (mode == 1U) {
        v = 1 - bg_change_tbl0[mode];
        switch (v) {
        case 0:
            dec_bg_handle = decdct(704, 0, 320, 160);
            v = 0;
            break;
        case 1:
            dec_bg_handle = decdct(320, 256, 320, 160);
            v = 1;
            break;
        }
        return v;
    }
}
