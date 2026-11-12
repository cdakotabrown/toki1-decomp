/* @SymbolStart: 0x80145DE4 */
/* @SymbolSize: 80 */
/* Address: 0x80145DE4 (cross-prologue head) / 0x80145DEC (body) */
/* Size: 80 bytes (20 instructions incl. the 2-word hoisted head) */
#include "types.h"

extern void date_name_set(u8);
extern void date_load3_ptr_set(void);
extern void date_main3_serifu_ptr_set(void);
extern void sub_sub_step_up(void);

extern u8 now_girl;
extern s16 date_now_serifu3;
extern s16 date_now_serifu3_in;

void date_main3_serifu_init(void) {
    date_now_serifu3 = 1;
    date_now_serifu3_in = 0;
    date_name_set(now_girl);
    date_load3_ptr_set();
    date_main3_serifu_ptr_set();
    sub_sub_step_up();
}
