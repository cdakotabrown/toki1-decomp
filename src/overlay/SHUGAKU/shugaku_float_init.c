/* @SymbolStart: 0x80135CA0 */
/* @SymbolSize: 192 */
/* @PostProc: scheduler_normalizer --target-bin assets/overlays/SHUGAKU.EXN --address 0x80135ca0 --block-aware --reloc-symbol-fp --const-fp --fence 18-30 */
/* shugaku_float_init -- SHUGAKU overlay, Address: 0x80135CA4, Size: 188 bytes. */
#include "types.h"

extern void sub_sub_step_up(void);

typedef struct {
    s32 w[17];
} Blk68;

extern u8 shugaku_float_on;
extern Blk68 cw96_active;
extern Blk68 cw99_active;
extern u8 cw99_prio;
extern u8 cw99_disp;
extern s16 cw99_anim;
extern u8 cw99_anim_flag;
extern s16 cw99_x;
extern s16 cw99_y;

void shugaku_float_init(void)
{
    u8 cur;

    shugaku_float_on = 1;
    cw99_active = cw96_active;

    cw99_prio = 11;
    cur = cw99_disp;
    cw99_disp = (u8)(cur | 0x80);
    cw99_anim = 5;
    cw99_anim_flag = 9;
    cw99_x = (s16)-96;
    cw99_y = (s16)-72;
    sub_sub_step_up();
}
