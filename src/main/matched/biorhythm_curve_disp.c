/* Function: biorhythm_curve_disp */
/* Address: 0x80068CA8 */
/* Size: 708 bytes (177 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count auto */
/* `@PostProc: custom_script func_80068CA8_postproc.py`, now retired and deleted. */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:11:deny IDO52_AS1_KEEP_DEAD_RELOAD=8,9 IDO52_AS1_NO_COPY_PROP=24,27 IDO52_FORCE_WEB_COLOR=14:v1,60:s2,95:v1 IDO52_AS1_TIE_REJECT=397,403,408,412,415,417,418,172,178 */
#include "common.h"

extern s32 frame_counter;
extern u16 pass_days;

extern void line_set(s16 x1, s16 y1, s16 x2, s16 y2, s32 color1, s32 color2, s32 mode);
extern s32 biorhythm_sf2(s32 a0, s32 a1, s32 a2, s32 a3);

void biorhythm_curve_disp(void) {
    s32 _pad0[2];
    u16 sp14E;
    s32 sp10C[16];
    s32 spCC[16];
    s32 sp8C[16];
    s32 sp88;
    s32 _padGap;
    s32 sp80;
    s32 i;
    s32 color;

    sp80 = frame_counter;

    if (sp80 & 0x80) {
        sp88 = (127 - (sp80 & 0x7F)) / 2;
    } else {
        sp88 = (sp80 & 0x7F) / 2;
    }

    color = sp88 * 0x10100;
    line_set(-128, -12, 124, -12, color, color, 129);

    sp14E = pass_days;
    pass_days = sp14E - 8;

    i = 0;

    do {
        sp10C[i] = biorhythm_sf2(1, 0, 0, -32);
        spCC[i] = biorhythm_sf2(0, 1, 0, -32);
        sp8C[i] = biorhythm_sf2(0, 0, 1, -32);

        pass_days++;

        if (i != 0) {
            s16 x1, x2;
            s32 colVal;
            s32 temp;

            x1 = (i * 16) - 144;
            x2 = (i * 16) - 128;

            temp = ((i + sp80) & 0xF);
            colVal = (temp * 4) + sp88;

            color = colVal * 257;
            line_set(x1, (s16)(sp10C[i - 1] - 12), x2, (s16)(sp10C[i] - 12), color, color, 129);

            color = colVal;
            line_set(x1, (s16)(spCC[i - 1] - 12), x2, (s16)(spCC[i] - 12), color, color, 129);

            color = colVal * 0x10001;
            line_set(x1, (s16)(sp8C[i - 1] - 12), x2, (s16)(sp8C[i] - 12), color, color, 129);
        }

        i++;
    } while (i != 16);

    pass_days = sp14E;
}
