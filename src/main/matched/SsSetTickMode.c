/* Address: 0x8008B93C */
/* Size: 460 bytes (115 instructions) */
/* Region: Library (0x80086000+) */
/* @SymbolStart: 0x8008B93C */
/* @SymbolSize: 460 */
/* @Compiler: ido52-gas */
/* @CompileFlags: --ido-fill-at-next --ido-copy-dead-ft --ido-copy-and-bump */
/* @CompileEnv: IDO52_UGEN_NO_REGION_MOVE=all IDO52_COPYPROP_SITE=lod:*:deny */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x70 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x74 */
#include "types.h"

extern s32 _snd_video_mode;
extern s32 VBLANK_MINUS;
extern s32 SsFCALL;
extern s32 _snd_seq_no_tick;

void SsSetTickMode(s32 mode) {
    s32 v;
    s32 sel;

    if (mode & 0x1000) {
        _snd_seq_no_tick = 1;
        v = mode & 0xFFF;
    } else {
        _snd_seq_no_tick = 0;
        v = mode;
    }
    if (v < 6) {
        SsFCALL = v;
        sel = v;
        switch (sel) {
        case 4:
            VBLANK_MINUS = 50;
            if (_snd_video_mode != 1) {
                v = 50;
                SsFCALL = v;
                return;
            } else {
                v = 5;
                SsFCALL = v;
                return;
            }
        case 1:
            VBLANK_MINUS = 60;
            goto c1_test;
        c1_0:
            v = 5;
            SsFCALL = v;
            return;
        c1_test:
            if (_snd_video_mode == 0) goto c1_0;
            v = 60;
            SsFCALL = v;
            return;
        case 3:
            VBLANK_MINUS = 120;
            return;
        case 2:
            VBLANK_MINUS = 240;
            return;
        c5_0:
            VBLANK_MINUS = 60;
            return;
        c5_1:
            VBLANK_MINUS = 50;
            return;
        case 5:
            if (_snd_video_mode == 0) goto c5_0;
            if (_snd_video_mode == 1) goto c5_1;
            VBLANK_MINUS = 60;
            return;
        c0_0:
            VBLANK_MINUS = 60;
            return;
        c0_1:
            VBLANK_MINUS = 50;
            return;
        case 0:
            if (_snd_video_mode == 0) goto c0_0;
            if (_snd_video_mode == 1) goto c0_1;
            VBLANK_MINUS = 60;
            return;
        default:
            VBLANK_MINUS = 60;
            return;
        }
    } else {
        VBLANK_MINUS = v;
        SsFCALL = v;
    }
}
