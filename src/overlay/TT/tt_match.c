/* tt_match - TT overlay, Address: 0x8013BB28, Size: 488 bytes (122 insns) */
/* @SymbolSize: 488 */
/* @PostProc: patch_stack_complete --old 48 --new 64 */
/* @PostProc: remap_stack_slots --from 40 --to 56 --from 36 --to 48 */
#include "types.h"

extern u8 *tt_step_wk;
extern u8 *tt_stage_wk;
extern u8 *tt_player_wk;
extern u8 *tt_boss_wk;

extern void device_cmd_send(s32 id);
extern void change_trans_pointer(s32 addr);
extern void tt_sprite_pool_clear(void);
extern void tt_cell_disp(void);
extern void tt_cell0_set(void);
extern void tt_cell1_set(void);
extern void tt_cell2_set(void);
extern void tt_cell3_set(void);
extern void tt_match_init2(void);
extern void tt_fall_step2(void);
extern void tt_match_move(void);
extern void tt_cutscene(void);
extern void tt_unit_step(void);
extern void tt_match_step(void);

void tt_match(void)
{
    u8 *s;
    u8 *r;
    u8 *w;
    u8 *m;

    s = tt_boss_wk;
    r = tt_step_wk;
    w = tt_player_wk;
    switch ((u32)*(u16 *)(s + 4)) {
    case 0:
        tt_sprite_pool_clear();
        if (*(u16 *)(r + 4) == 0) {
            device_cmd_send(177);
            change_trans_pointer(0x8019A000);
            device_cmd_send(18);
            device_cmd_send(35);
            device_cmd_send(46);
            device_cmd_send(33);
            device_cmd_send(4);
            device_cmd_send(512);
        }
        tt_match_init2();
        *(s16 *)(s + 4) = 32;
        break;
    case 32:
        tt_cell0_set();
        tt_cell1_set();
        tt_cell2_set();
        tt_cell3_set();
        tt_fall_step2();
        tt_unit_step();
        break;
    case 64:
        tt_match_move();
        tt_unit_step();
        tt_match_step();
        tt_cell0_set();
        tt_cell1_set();
        tt_cell2_set();
        tt_cell3_set();
        if (*(u16 *)(s + 782) == 1 && *(s16 *)(s + 780) < 0) {
            *(s16 *)(s + 4) = 128;
            *(s16 *)(s + 10) = 0;
            *(s16 *)(s + 8) = 0;
            *(s16 *)(s + 6) = 0;
            s[15] = 0;
            s[14] = 0;
            s[13] = 0;
        }
        break;
    case 128:
        device_cmd_send(193);
        w[82] = 1;
        tt_cutscene();
        break;
    case 160:
        device_cmd_send(177);
        s[0] = 0;
        r[18] = 0;
        m = tt_stage_wk;
        *(s16 *)(m + 30) = 1;
        *(s16 *)(r + 2) = 160;
        break;
    }
    tt_cell_disp();
}
