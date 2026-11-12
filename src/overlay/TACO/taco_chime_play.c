/* @PostProc: patch_stack_complete --old 56 --new 64 */
/* @PostProc: remap_stack_slots --from 44 --to 56 */
/* @PostProc: patch_insn_reg_field 0x30:rt:v0:v1 0x38:rs:v0:v1 0x58:rd:v0:v1 0x5c:rs:v0:v1 0x8c:rt:v0:v1 0x94:rs:v0:v1 0x98:rd:v0:v1 0xa8:rs:v0:v1 */
/* @SymbolStart: 0x8015182C */
/* @SymbolSize: 972 */
/* Address: 0x80151834 */
/* Size: 964 bytes (241 insns) */
#include "types.h"

extern u8 *taco_actor_rec;

extern s32 rand(void);
extern void sd_cmd_limit_send(s32, s32);
extern void taco_node_shot_spawn(s32, s32, s32, s32, s32, s32, s32);

void taco_chime_play(void)
{
    u8 *p;
    u32 r;
    s32 x;
    s32 y;

    p = taco_actor_rec;
    if (p[2309] != 0) {
        r = (u32)p[2178] % 10;
        if (r == 0) {
            taco_node_shot_spawn(0, 25, 0, 0, 1000, 150, 197);
            sd_cmd_limit_send(0, 1281);
            r = (u32)taco_actor_rec[2178] % 10;
        }
        if (r == 5) {
            taco_node_shot_spawn(0, 25, 0, 0, 1000, 150, 197);
        }
    } else if (taco_actor_rec[2445] != 0) {
        if ((u32)taco_actor_rec[2178] % 5 == 1) {
            taco_node_shot_spawn(0, 25, 0, -700, 1000, 300, 196);
        }
    } else if (taco_actor_rec[2581] != 0) {
        if ((taco_actor_rec[2178] & 3) == 1) {
            taco_node_shot_spawn(0, 25, 0, -1500, 1000, 400, 132);
        }
    } else if ((u32)taco_actor_rec[2178] % 3 == 1) {
        x = rand();
        y = rand();
        taco_node_shot_spawn(0, 25, (s16)((x & 0x1FF) - 256),
                      (s16)((y & 0x1FF) - 256), 1000, 400, 132);
    }

    if (taco_actor_rec[2717] == 0 || taco_actor_rec[2853] == 0) {
        if ((taco_actor_rec[2178] & 1) == 0) {
            if (taco_actor_rec[2989] != 0) {
                taco_node_shot_spawn(0, 13, 0, 0, 1000, 400, 129);
            }
            if (taco_actor_rec[3397] != 0) {
                taco_node_shot_spawn(0, 19, 0, 0, 1000, 400, 129);
            }
            if (taco_actor_rec[3125] != 0) {
                taco_node_shot_spawn(0, 15, 0, 0, 1000, 400, 129);
            }
            if (taco_actor_rec[3533] != 0) {
                taco_node_shot_spawn(0, 21, 0, 0, 1000, 400, 129);
            }
            if (taco_actor_rec[3261] != 0) {
                taco_node_shot_spawn(0, 17, 0, 0, 1000, 400, 129);
            }
            if (taco_actor_rec[3669] != 0) {
                taco_node_shot_spawn(0, 23, 0, 0, 1000, 400, 129);
            }
        }
    }
}
