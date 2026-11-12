/* @PostProc: patch_stack_complete --old 48 --new 56 */
/* @PostProc: remap_stack_slots --from 24 --to 44 --from 32 --to 52 */
/* @SymbolStart: 0x80134D98 */
/* @SymbolSize: 344 */
#include "types.h"

extern s32 rand(void);
extern s32 en_nichi_sq_dist(s32 a, s32 b, s32 c, s32 d);
extern s32 en_nichi_dir_get(s32);
extern void en_nichi_obj_dir0_turn(s32 a, s32 b);
extern void en_nichi_obj_dir1_turn(s32 a, s32 b);
extern void func_801351C0(s32 a, s32 b);
extern void en_nichi_obj_dir3_turn(s32 a, s32 b);
extern void en_nichi_obj_move_select(s32);

extern u8 en_nichi_obj_wk[];
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;

void en_nichi_obj_react(s32 idx)
{
    s32 *rec;
    s32 flags;
    s32 ret;
    s32 tmp;

    rec = (s32 *)(en_nichi_obj_wk + idx * 48);
    flags = rec[11];
    if (flags & 0x40) {
        goto check;
    }
    if (!(flags & 0x80)) {
        goto after_ctrl;
    }
check:
    if (rec[9] > 0) {
        goto after_ctrl;
    }
    rec[10] = (rec[2] << 1) + 4;
    rec[9] = rand() % 15 + 30;
    if (!(rand() & 1)) {
        rec[11] = 0;
    } else {
        rec[11] = 1;
    }

after_ctrl:
    tmp = en_nichi_sq_dist(*(s16 *)((u8 *)rec + 22),
                        *(s16 *)((u8 *)rec + 26),
                        cursor_cw_x, cursor_cw_y);
    ret = en_nichi_dir_get(idx);
    switch (ret) {
    case 0:
        en_nichi_obj_dir0_turn(idx, tmp);
        break;
    case 1:
        en_nichi_obj_dir1_turn(idx, tmp);
        break;
    case 2:
        func_801351C0(idx, tmp);
        break;
    case 3:
        en_nichi_obj_dir3_turn(idx, tmp);
        break;
    }
    en_nichi_obj_move_select(idx);
}
