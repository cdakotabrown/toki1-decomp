/* @SymbolStart: 0x80135260 */
/* @SymbolSize: 84 */
#include "types.h"

extern void kangei_girl_msg_init(void);
extern void sub_sub_step_up(void);
extern u8 now_girl;
extern u8 girl0_yuukou[];
extern s16 kangei_now_s;

void kangei_yuukou_set70(void)
{
    *(s16 *)(girl0_yuukou + (u32)now_girl * 56) = 70;
    kangei_now_s = 3;
    kangei_girl_msg_init();
    sub_sub_step_up();
}
