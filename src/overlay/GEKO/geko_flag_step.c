/* @SymbolStart: 0x80136E28 */
/* @SymbolSize: 104 */
#include "types.h"

typedef struct {
    unsigned lo : 3;
    unsigned b3 : 1;
    unsigned hi : 28;
    u8 pad[52];
} Rec56;

extern u8 now_girl;
extern Rec56 girl0_flag[];
extern void sub_sub_step_up(void);
extern void geko_goto_sub31(void);

extern s16 geko_now_serifu2;

void geko_flag_step(void)
{
    if (girl0_flag[now_girl].b3) {
        geko_now_serifu2 = geko_now_serifu2 + 1;
        sub_sub_step_up();
    } else {
        geko_goto_sub31();
    }
}
