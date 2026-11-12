/* @SymbolStart: 0x80151C00 */
/* @SymbolSize: 236 */
/* @PostProc: patch_stack_complete --old 48 --new 80 */
/* @PostProc: remap_stack_slots --from 40 --to 60 */
/* taco_panel_main -- TACO overlay, Address: 0x80151C08 (true start 0x80151C00: */
#include "types.h"

extern u8 *taco_actor_rec;
extern s32 rand(void);
extern void taco_node_shot_spawn(s32, s32, s32, s32, s32, s32, s32);
extern void taco_panel_anim(s32);
extern void taco_anim_step(s32);
extern void taco_panel(s32);
extern void taco_slots_main(void);
extern void taco_panel_frame_step(void);

void taco_panel_main(void)
{
    u8 *p;
    s32 a;
    s32 b;

    p = taco_actor_rec;
    if (p[2310] == 0) {
        taco_panel_anim(5);
    }
    taco_anim_step(6);
    taco_panel(4);
    taco_slots_main();
    taco_panel_frame_step();

    p = taco_actor_rec;
    if (p[2309] == 0 && p[2445] == 0 && p[2581] == 0) {
        a = rand();
        b = rand();
        taco_node_shot_spawn(0, 28, (s16)((a & 0x1FF) - 256),
                      (s16)((b & 0x1FF) - 256), 1000, 400, 196);
    }
}
