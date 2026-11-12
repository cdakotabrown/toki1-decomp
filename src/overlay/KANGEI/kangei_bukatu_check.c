/* @SymbolStart: 0x80137640 */
/* @SymbolSize: 100 */
#include "types.h"

extern void sub_step_up(void);
extern void sub_sub_step_up(void);

extern u16 basyo_pack;
typedef struct {
    unsigned lo : 25;
    unsigned f : 2;
    unsigned hi : 5;
} Word;

extern Word bukatu_rec[];
extern u8 update_anim_state_tbl4;

s32 kangei_bukatu_check(void)
{
    if (bukatu_rec[(u32)basyo_pack >> 12].f < 3U) {
        sub_step_up();
        return 0;
    }
    update_anim_state_tbl4 = 10;
    sub_sub_step_up();
}
