/* tt_board_scene_main - TT overlay, Address: 0x80132CCC, Size: 572 bytes (143 insns) */
/* @SymbolSize: 572 */
/* @PostProc: patch_stack_complete --old 48 --new 64 */
/* @PostProc: remap_stack_slots --from 44 --to 60 --from 40 --to 52 --from 32 --to 48 */
/* @PostProc: patch_insn_reg_field 0x18:rt:t7:t8 0x20:rs:t7:t8 0x20:rt:t7:t8 0x28:rt:t8:t7 0x2c:rs:t8:t7 0x38:rt:t7:t8 0x68:rt:t9:t0 0x164:rt:t9:t0 0x17c:rt:t7:t8 0x180:rs:t7:t8 0x190:rt:v0:t6 0x198:rs:v0:t6 0x1a8:rt:t6:t7 0x1ac:rs:t6:t7 0x1b0:rt:t8:t9 0x1b4:rs:t8:t9 0x1e0:rt:t9:t0 0x1e4:rt:t0:t1 0x1e8:rs:t9:t0 0x1f0:rs:t0:t1 0x1f0:rt:t1:t2 0x1f4:rt:t2:t3 0x1f8:rs:t1:t2 0x1fc:rt:t7:t6 0x200:rs:t2:t3 0x200:rt:t3:t4 0x204:rt:t4:t5 0x208:rs:t3:t4 0x20c:rt:t5:t8 0x214:rt:t4:t5 0x21c:rt:t5:t8 0x220:rt:t7:t6 */
#include "types.h"

extern u8 *tt_game_wk;
extern u8 *tt_step_wk;
extern u8 *tt_scroll_wk;

extern void device_cmd_send(s32 id);
extern void tt_widget_init(void);
extern void tt_row_init(s32 a, s32 b, s32 c);
extern void tt_board_init(s32 a, s32 b);
extern void tt_board_scroll(void);
extern void tt_intro_step(void);
extern void tt_outro_step(void);
extern void tt_obj_clear(void);
extern void tt_shot_clear(void);
extern void tt_active_clear(void);
extern void tt_slot_clear(void);
extern void tt_ent_clear(void);
extern void tt_rec_clear(void);
extern void trail_clear_all(void);
extern void tt_text_clear(void);
extern void tt_recs_clear(void);
extern void tt_player_shot_clear(void);
extern void tt_pool_clear(void);
extern void tt_record_clear(void);
extern void tt_sprite_pool_reset(void);

void tt_board_scene_main(void)
{
    u8 *p;
    u8 *u;
    u8 *s;
    u8 *q;
    s32 f;
    s32 n;
    s32 k;

    p = tt_game_wk;
    u = tt_scroll_wk;
    s = tt_step_wk;
    q = p + 16;
    switch (*(u16 *)(s + 2)) {
    case 0:
        device_cmd_send(34);
        device_cmd_send(47);
        device_cmd_send(4);
        device_cmd_send(516);
        tt_record_clear();
        tt_text_clear();
        tt_active_clear();
        tt_player_shot_clear();
        tt_rec_clear();
        tt_recs_clear();
        trail_clear_all();
        tt_pool_clear();
        tt_obj_clear();
        tt_shot_clear();
        tt_sprite_pool_reset();
        tt_ent_clear();
        tt_slot_clear();
        s[17] = 0;
        s[28] = 1;
        s[29] = 0;
        s[30] = 0;
        s[31] = 0;
        tt_widget_init();
        tt_row_init(2, 6, 4);
        tt_board_init(13, 0);
        *(s16 *)(s + 46) = 0;
        if (s[34] != 0) {
            *(s16 *)(s + 38) = 300;
        } else {
            *(s16 *)(s + 38) = 3600;
        }
        *(s16 *)(s + 2) = 32;
        break;
    case 32:
        tt_intro_step();
        break;
    case 64:
        tt_board_scroll();
        *(u16 *)(s + 38) = *(u16 *)(s + 38) - 1;
        n = 128;
        if (*(u16 *)(s + 38) == 0) {
            *(s16 *)(s + 2) = (s16)n;
        }
        k = s[34];
        n = 128;
        if (k == 0) {
            f = *(u16 *)(q + 4);
            if ((f & 0x0020) != 0 || (f & 0x0800) != 0) {
                s[29] = (u8)n;
                s[30] = (u8)n;
                s[31] = (u8)n;
                *(s16 *)(s + 2) = (s16)n;
            }
        }
        break;
    case 128:
        device_cmd_send(177);
        tt_outro_step();
        break;
    }
    if (s[9] != 0) {
        if (p[83] == 0) {
            if (u[15] != 0) {
                *(s16 *)s = 8;
            } else {
                *(s16 *)s = 1;
            }
        } else {
            *(s16 *)s = 1;
        }
        *(s16 *)(s + 6) = 0;
        *(s16 *)(s + 4) = 0;
        *(s16 *)(s + 2) = 0;
        s[11] = 0;
        s[10] = 0;
        s[9] = 0;
    }
}
