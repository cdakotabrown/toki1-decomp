/* @SymbolStart: 0x8013EFA4 */
/* @SymbolSize: 136 */
/* Address: 0x8013EFA4 (cross-prologue head absorbed), Size: 140 bytes */
#include "types.h"

extern u8 *tt_db_buf;
extern u8 *tt_disp_wk;

extern void ResetGraph(s32 param);
extern void PutDrawEnv(u8 *ptr);
extern void PutDispEnv(u8 *ptr);
extern void DrawOTag(u8 *ptr);

void tt_dbuf_show(void)
{
    s32 pad;
    u8 *base;

    base = tt_db_buf + *(s32 *)(tt_disp_wk + 8) * 32952;

    ResetGraph(1);
    PutDrawEnv(base + 0x8048);
    PutDispEnv(base + 0x80A4);
    DrawOTag(base + 0x8044);
}
