/* @CompileEnv: IDO52_FORCE_WEB_COLOR=0:a2,11:a3 */
/* @SymbolStart: 0x8013BE9C */
/* @SymbolSize: 204 */
#include "types.h"

extern s32 tt_cmd_once();
extern u8 *tt_obj_alloc(u8 *a0, u8 *a1);
extern void tt_obj_kind_set(u8 *a0, s32 a1);
extern s32 tt_obj_anim_tbl;
extern u8 *tt_bell_buf;

void tt_effect_make(u8 *src)
{
    u8 *base = tt_bell_buf;
    u8 *obj;

    tt_cmd_once(1542);
    obj = tt_obj_alloc(base, base + 3456);

    *(s32 *)(obj + 32) = *(s32 *)(src + 12);
    *(s32 *)(obj + 36) = *(s32 *)(src + 16);
    *(s16 *)(obj + 20) = *(s16 *)(src + 0);
    *(s16 *)(obj + 22) = *(s16 *)(src + 2);
    *(s32 *)(obj + 48) = 0;
    *(s32 *)(obj + 52) = 0;
    *(s32 *)(obj + 44) = 0;
    *(s32 *)(obj + 40) = 0;
    obj[0] = 1;
    *(s32 *)(obj + 56) = tt_obj_anim_tbl;
    obj[64] = 128;
    *(s16 *)(obj + 16) = 6;
    *(s16 *)(obj + 98) = 0;
    *(s16 *)(obj + 80) = 0;
    *(s16 *)(obj + 4) = 144;
    obj[13] = 0;
    obj[14] = 0;
    obj[15] = 0;
    *(s16 *)(obj + 6) = 0;
    *(s16 *)(obj + 8) = 0;
    *(s16 *)(obj + 10) = 0;

    tt_obj_kind_set(obj, 44);
}
