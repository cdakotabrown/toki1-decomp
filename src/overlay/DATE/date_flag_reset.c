/* @SymbolStart: 0x8014FA20 */
/* @SymbolSize: 176 */
/* Address: 0x8014FA94 */
/* Size: 60 bytes (15 instructions) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern void date_ptr_init(void);
extern s32 serifu_no;
extern s16 kao[];
extern s16 kao2[];
extern s16 imp[];
extern s16 ans[];
extern s16 now_serifuev;
extern s16 now_serifuev_in;

void date_flag_reset(void) {
    serifu_no = 0;
    kao[0] = 1;
    kao[1] = 1;
    kao[2] = 1;
    ans[0] = 0;
    ans[1] = 0;
    ans[2] = 0;
    kao2[0] = 1;
    kao2[1] = 1;
    kao2[2] = 1;
    imp[0] = 0;
    imp[1] = 0;
    imp[2] = 0;
    sub_sub_step_up();
    now_serifuev = 0;
    now_serifuev_in = 0;
    date_ptr_init();
}
