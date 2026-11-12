/* Address: 0x8014BEAC (absorbed cross-prologue head) .. 0x8014BF98 */
/* Size: 236 bytes (59 instructions) */
/* @SymbolStart: 0x8014BEAC */
/* @SymbolSize: 236 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 get_CD_State2(void);
extern s32 indexed_byte_lsb_set(s32, s32);
extern s32 value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);

extern volatile u8 step_mode;

s32 etc_soudan_bg_load(void)
{
    s32 r;
    s32 sel;
    s32 x;

    sel = step_mode;
    r = sel;
    switch (sel) {
    case 0:
        r = x = value_cache_find_or_load(0x408D, 1);
        if (x != -1) {
            if (x == 0 || x == 1) {
                sel = step_mode;
                step_mode = sel + 2;
                r = indexed_byte_lsb_set(1, x);
            }
        } else {
            sel = step_mode;
            step_mode = sel + 1;
        }
        break;
    case 1:
        r = get_CD_State2();
        if (r & 1) {
            r = dec_bg_init(1);
            r = indexed_byte_lsb_set(1, r);
            sel = step_mode;
            step_mode = sel + 1;
        }
        break;
    case 2:
        r = sub_sub_step_up();
        break;
    }
    return r;
}
