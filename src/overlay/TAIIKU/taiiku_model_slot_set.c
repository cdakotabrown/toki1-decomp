/* @SymbolSize: 116 */
/* @CompileEnv: IDO52_FORCE_WEB_RANGE_SPLIT=4 IDO52_FORCE_WEB_NOSPILL=4.0:1,4.1:1 IDO52_FORCE_WEB_COLOR=4:a0,18:a3 */
#include "types.h"

extern s32 object[][4];
extern u8 coord_buf[][80];

extern void GsMapModelingData(s32 *);
extern void GsLinkObject4(s32 *, s32 *, s32);

void taiiku_model_slot_set(s32 *arg0, s32 arg1)
{
    s32 *q;

    q = &arg0[1];
    GsMapModelingData(q);
    GsLinkObject4(q + 2, object[arg1], 0);
    object[arg1][1] = (s32)coord_buf[arg1];
    object[arg1][0] = 0;
}
