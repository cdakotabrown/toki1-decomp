/* @SymbolStart: 0x80132760 */
/* @SymbolSize: 204 */
#include "types.h"

extern u8 ret_sub_step;

extern u8  shugaku_place;
extern u8  basho_num;
extern s32 fade_flag;
extern u8  fade_bri;

extern void event_log_add(s32 a0, s32 a1);
extern void shugaku_res_init(void);
extern void shugaku_birth_serifu_set(void);

void shugaku_birth_init(void)
{
    s32 t6_in = ret_sub_step;

    u8  d07cc;
    u8  flag;
    s32 ptr;

    if (t6_in != 6) {
        event_log_add(10, 41);
        goto label_end;
    }

    d07cc = shugaku_place;
    switch (d07cc) {
    case 0: event_log_add(10, 42); break;
    case 1: event_log_add(10, 43); break;
    default: event_log_add(10, 44); break;
    }

label_end:
    shugaku_res_init();
    shugaku_birth_serifu_set();

    flag = fade_bri;
    if (flag != 0) {
        ptr = fade_flag;
        if (ptr != 0) {
            basho_num = 1;
            return;
        }
    }
    basho_num = 0;
}
