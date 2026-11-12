/* @SymbolSize: 136 */
/* Address: 0x80146A24 */
/* Size: 136 bytes (34 instructions) */
#include "types.h"

extern s32 dec_bg_handle;

extern s32 get_CD_State2(void);
extern s32 dec_bg_init(s32);
extern void indexed_byte_lsb_set(s32, s32);
extern void dec_bg_reset(void);
extern void value_cache_find_or_load(s32, s32);
extern void set_dec_bri(s32);
extern void sub_sub_step_up(void);
extern void dec_bg_show(s32);

void etc_bg_load(void)
{
    s32 ret;

    if (get_CD_State2() & 1) {
        ret = dec_bg_init(0);
        indexed_byte_lsb_set(0, ret);
        if (dec_bg_handle == -1) {
            dec_bg_reset();
            value_cache_find_or_load(16292, 0);
        } else {
            set_dec_bri(128);
            sub_sub_step_up();
            dec_bg_show(0);
        }
    }
}
