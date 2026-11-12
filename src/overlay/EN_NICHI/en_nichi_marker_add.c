/* en_nichi_marker_add - EN_NICHI overlay, Address: 0x80135BC4, Size: 132 bytes (33 insns) */
/* @CompileEnv: IDO52_COPYPROP_VAR=-4,-8 IDO52_LDA_NOINREG_CLASSES=1,4 IDO52_COPYPROP_SITE=lod:1:deny,lod:2:deny IDO52_AS1_TIE_TAKE=40 */
#include "types.h"

extern s32 en_nichi_marker_no;
extern s32 en_nichi_marker_timer[];
extern s16 en_nichi_marker_tbl[][4];

s32 en_nichi_marker_add(s32 arg0, s32 arg1)
{
    s32 idx;
    s32 *p;
    s32 v;

    idx = en_nichi_marker_no;
    p = &en_nichi_marker_timer[idx];
    v = *p;
    if (v > 0) {
        if (v < 10) {
            return;
        }
    }
    idx = idx + 1;
    p = p + 1;
    if (idx >= 10) {
        p = en_nichi_marker_timer;
        idx = 0;
    }
    en_nichi_marker_tbl[idx][1] = arg0 + 160;
    en_nichi_marker_tbl[idx][3] = arg1 + 120;
    *p = 1;
    en_nichi_marker_no = idx;
}
