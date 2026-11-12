/* @PostProc: patch_stack_complete --old 40 --new 56 */
/* @PostProc: swap_registers v1:a1 --bidirectional */
/* @PostProc: remap_stack_slots --map '24:48,28:40,32:44,36:52' */
/* @PostProc: swap_instructions --offset1 0x94 --offset2 0x9c */
/* @PostProc: swap_instructions --offset1 0xac --offset2 0xb4 */
/* @PostProc: swap_instructions --offset1 0xd0 --offset2 0xd4 */
/* @PostProc: pad_trailing_nop --count 1 */
/* @PostProc: patch_insn_reg_field 0x38:rt:t9:t0 0x3c:rt:t0:t9 0x138:rt:t7:a0 0x154:rt:t7:a0 0x170:rt:t0:t7 0x174:rt:t1:t0 0x178:rs:t0:t7 0x184:rs:t1:t0 0x184:rt:t1:t0 0x188:rt:t8:t1 0x18c:rs:t1:t0 0x190:rs:t1:t0 0x194:rs:t1:t0 0x198:rs:t1:t0 0x19c:rs:t1:t0 0x1a0:rs:t1:t0 0x1a4:rs:t1:t0 0x1a8:rs:t1:t0 0x1b0:rt:t8:t1 0x1b4:rt:t2:t8 0x1bc:rs:t2:t8 0x1f0:rt:v0:t2 0x1f8:rs:v0:t2 0x268:rt:t2:t9 0x270:rs:t2:t9 0x278:rt:v0:t2 0x27c:rt:t4:t5 0x280:rs:v0:t2 0x280:rt:t9:t3 0x284:rs:t9:t3 0x288:rs:v0:t2 0x288:rt:t3:t4 0x28c:rs:t3:t4 0x294:rt:t4:t5 */
#include "types.h"

extern u8 *tt_game_wk;
extern u8 *tt_step_wk;
extern u8 *tt_scroll_wk;
extern u8 *tt_stage_wk;
extern u8 *tt_boss_wk;
extern u8 *tt_text_buf;

extern s32 device_cmd_send();
extern s32 tt_actor_pool_step(void);
extern s32 obj_pool_step(void);
extern s32 tt_speed_mark_set();
extern s32 tt_pad_timeout_step();
extern s32 tt_sprite_pool_step(void);
extern s32 tt_scroll_speed_step();
extern s32 tt_rec_step(void);
extern s32 tt_ent_step_all(void);
extern s32 tt_recs_step(void);
extern s32 tt_sprite_step(void);
extern s32 tt_slot_step(void);

void tt_rally_scene_main(void)
{
    u8 *ptrA70;
    u8 *ptrA7C;
    u8 *ptrA80;
    u8 *ptrA74;
    s32 sel;
    s32 b9;
    s32 h2;
    u8 *rec;
    u8 *w;
    s32 fp;

    ptrA70 = tt_game_wk;
    ptrA7C = tt_scroll_wk;
    ptrA80 = tt_stage_wk;
    ptrA74 = tt_step_wk;

    *(s32 *)(ptrA80 + 0) = *(s32 *)(ptrA80 + 0) + *(s32 *)(ptrA7C + 0);

    sel = *(u16 *)(ptrA74 + 4);

    switch ((u32)sel) {
    case 0:
        if (*(u8 *)(ptrA74 + 34) != 0 && *(u8 *)(ptrA70 + 82) == 255) {
            device_cmd_send(177);
            *(u8 *)(ptrA74 + 17) = 0;
        } else {
            device_cmd_send(34);
            device_cmd_send(47);
            device_cmd_send(4);
            device_cmd_send(515);
            *(u8 *)(ptrA7C + 15) = 1;
            *(u8 *)(ptrA74 + 17) = 0;
        }

        {
            rec = tt_text_buf + 2520;

            *(s16 *)(tt_text_buf + 2530) = -36;
            *(s16 *)(rec + 12) = 116;
            *(s16 *)(rec + 14) = 9;
            *(u8 *)(rec + 2) = 2;
            *(u8 *)(rec + 0) = 1;
            *(s16 *)(rec + 6) = 0;
            *(s16 *)(rec + 16) = 0;
        }

        if ((u32)*(u16 *)(ptrA80 + 2) >= 1025U) {
            *(s32 *)(ptrA7C + 4) = 0;
        } else {
            *(s32 *)(ptrA7C + 4) = 0x8000;
        }
        *(s16 *)(ptrA74 + 38) = 0;
        *(s16 *)(ptrA74 + 4) = 64;
        break;
    case 64:
        h2 = *(u16 *)(ptrA74 + 2);

        switch ((u32)h2) {
        case 64:
        default:
            if ((u32)*(u16 *)(ptrA80 + 2) >= 1025U) {
                w = tt_boss_wk;

                *(s16 *)(w + 4) = 0;
                *(u8 *)(w + 12) = 0;
                *(s16 *)(w + 10) = 0;
                *(s16 *)(w + 8) = 0;
                *(s16 *)(w + 6) = 0;
                *(u8 *)(w + 15) = 0;
                *(u8 *)(w + 14) = 0;
                *(u8 *)(w + 13) = 0;
                *(s16 *)(ptrA74 + 2) = 80;
            }
            break;
        case 80:
            if (*(u16 *)(ptrA80 + 30) == 0) {
                (*(void (**)(void))(ptrA80 + 12))();
            }
            break;
        }

        tt_pad_timeout_step();

        b9 = *(u8 *)(ptrA74 + 9);
        if (b9 != 0) {
            if (*(u8 *)(ptrA74 + 34) != 0) {
                if (*(u8 *)(ptrA70 + 82) == 255) {
                    *(s16 *)(ptrA74 + 0) = 1;
                } else {
                    *(s16 *)(ptrA74 + 0) = 8;
                }
            } else {
                *(s16 *)(ptrA74 + 0) = 7;
            }
            *(s16 *)(ptrA74 + 6) = 0;
            *(s16 *)(ptrA74 + 4) = 0;
            *(s16 *)(ptrA74 + 2) = 0;
            *(u8 *)(ptrA74 + 11) = 0;
            *(u8 *)(ptrA74 + 10) = 0;
            *(u8 *)(ptrA74 + 9) = 0;
        }
        break;
    }

    if (*(u8 *)(ptrA74 + 34) != 0 && *(u8 *)(ptrA70 + 82) == 255) {
        s32 f20 = *(u16 *)(ptrA70 + 20);

        if ((f20 & 0x0800) != 0 || (f20 & 0x0020) != 0) {
            *(s16 *)(ptrA74 + 0) = 1;
            *(s16 *)(ptrA74 + 6) = 0;
            *(s16 *)(ptrA74 + 4) = 0;
            *(s16 *)(ptrA74 + 2) = 0;
            *(u8 *)(ptrA74 + 11) = 0;
            *(u8 *)(ptrA74 + 10) = 0;
            *(u8 *)(ptrA74 + 9) = 0;
        }
    }

    tt_sprite_step();
    tt_rec_step();
    tt_recs_step();
    tt_sprite_pool_step();
    tt_ent_step_all();
    tt_slot_step();
    tt_actor_pool_step();
    obj_pool_step();
    tt_scroll_speed_step();
    tt_speed_mark_set();
}
