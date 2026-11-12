/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x80146B2C */
/* @SymbolSize: 132 */
/* Address: 0x80146B34, Size: 124 bytes */
#include "types.h"

extern s32 etc_exam_week_init(void);
extern s32 etc_exam_week_bg_load(void);
extern s32 etc_bg_load(void);
extern s32 etc_week_score_clear(void);

extern u8 now_sub_sub_step;

s32 etc_sub_sub_main0(void)
{
    s32 ret;

    ret = now_sub_sub_step;
    switch (now_sub_sub_step) {
    case 0:
        ret = etc_exam_week_init();
        break;
    case 1:
        ret = etc_exam_week_bg_load();
        break;
    case 2:
        ret = etc_bg_load();
        break;
    case 3:
        ret = etc_week_score_clear();
        break;
    }
    return ret;
}
