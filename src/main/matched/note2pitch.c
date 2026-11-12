/* Address: 0x8008D580 */
/* Size: 216 bytes (54 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -O2 (native as1) */
/* @CompileFlags: -Wc,-pic0 -cckr -Wo,-nordstore */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_CONST_NOINREG_VALUES=12 IDO52_AS1_DIV_POW2=aspsx IDO52_AS1_NO_SHIFT_COMBINE=all IDO52_AS1_KEEP_SEXT=all IDO52_AS1_NO_JUMP_CHAIN=2 IDO52_COPYPROP_SITE=lod:1:deny IDO52_COPYPROP_VAR=-4 IDO52_FORCE_WEB_COLOR=0:a2,14:a1,18:t0,16:a3,20:a0,28:a1 IDO52_UGEN_TEMP_REG=1:v0,2:v1,3:a1 IDO52_UGEN_TEMP_REG_INPLACE=occ:1,2,3 IDO52_AS1_KEEP_DEAD_RELOAD=1,3 IDO52_AS1_XBB_SKIP_CAND=2 IDO52_AS1_NO_FALLTHRU_FILL=1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x5c */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x7c */
#include "types.h"

extern s8  D_801315F2;
extern s8  libsnd_cur_tone_center;
extern s8  libsnd_cur_tone_shift;
extern u16 libsnd_note_pitch_tbl[];

u16 note2pitch(void)
{
    int semitones;
    int fine;
    short octave;
    unsigned short step;
    unsigned short pitch;

    semitones = (short)(D_801315F2 - libsnd_cur_tone_center + 60);
    fine = libsnd_cur_tone_shift / 8;
    octave = semitones / 12;
    step = fine;
    if (step >= 16) {
        step = 15;
    }
    pitch = libsnd_note_pitch_tbl[(short)(semitones % 12) * 16 + step];
    octave = octave - 5;
    if (octave > 0) {
        pitch <<= octave;
    } else if (octave < 0) {
        pitch >>= -octave;
    }
    return pitch;
}
