/* @RodataBase: 0x8015F15C */
/* @PostProc: patch_stack_complete --old 88 --new 104 */
/* Address: 0x801509A4 */
/* Size: 472 bytes (118 insns) */
/* @SymbolSize: 472 */
/* @PostProc: remap_stack_slots --from 44 --to 48 */
/* @PostProc: remap_stack_slots --from 43 --to 99 */
#include "types.h"

typedef struct {
    s32 v[11];
} Slots44;

extern Slots44 bunkasai_girl_select_tbl;
extern s32 vblnk_counter;
extern u8  screen_w[];
extern s32 bunka_girl;

extern void goto_sub_step(s32);
extern void game_reset(void);
extern s32 rand(void);
extern void srand(s32);

void bunkasai_girl_select(void) {
    Slots44 slots;
    u8 count;
    s32 i;
    s32 pick;
    u8 *ent;

    slots = bunkasai_girl_select_tbl;
    srand(vblnk_counter);
    count = 0;
    ent = screen_w;
    for (i = 0; i != 11; i++) {
        if ((u32)(*(s32 *)(ent + 456) << 30) >> 31 == 1) {
            slots.v[count] = i;
            count = count + 1;
        }
        ent += 56;
    }
    pick = slots.v[(u32)rand() % count];
    bunka_girl = pick;
    switch (pick) {
    case 0:
        goto_sub_step(15);
        break;
    case 1:
        goto_sub_step(28);
        break;
    case 2:
        goto_sub_step(46);
        break;
    case 3:
        goto_sub_step(65);
        break;
    case 4:
        goto_sub_step(85);
        break;
    case 5:
        goto_sub_step(97);
        break;
    case 6:
        goto_sub_step(109);
        break;
    case 7:
        goto_sub_step(121);
        break;
    case 8:
        goto_sub_step(133);
        break;
    case 9:
        goto_sub_step(145);
        break;
    case 10:
        goto_sub_step(154);
        break;
    default:
        game_reset();
        break;
    }
}
