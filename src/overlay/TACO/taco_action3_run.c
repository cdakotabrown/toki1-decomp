/* @PostProc: swap_instructions --offset1 0xf8 --offset2 0x110 */
/* @PostProc: swap_instructions --offset1 0x104 --offset2 0x110 */
/* @PostProc: swap_instructions --offset1 0x108 --offset2 0x110 */
/* @PostProc: swap_instructions --offset1 0x10c --offset2 0x110 */
/* @PostProc: swap_instructions --offset1 0x278 --offset2 0x27c */
/* @PostProc: swap_instructions --offset1 0x27c --offset2 0x280 */
/* Address: 0x8014E170 */
/* Size: 772 bytes (193 insns) */
#include "types.h"

typedef struct {
    u8 pad0[1];
    u8 f1;
    u8 pad1[118];
    s16 f120;
    u8 pad2[10];
    u8 f132;
    u8 pad3[3];
} Rec;

extern Rec *taco_actor_rec;

extern void taco_rec_move(s32, s32, s32, s32, s32, s32, s32, s32, s32);

void taco_action3_run(s32 t)
{
    if (t < 20) {
        taco_rec_move(0, 0, 0, -300, 0, 0, 0, 50, 50);
    } else if (t >= 20 && t < 47) {
        taco_rec_move(0, 0, 0, 800, 0, 0, 0, 20, 30);
    } else if (t >= 50 && t < 80) {
        taco_rec_move(0, 0, 0, -520, 0, 0, 0, 50, 50);
    }

    if (t >= 45 && t < 55) {
        taco_actor_rec[17].f1 = 64;
        taco_actor_rec[17].f120 = 2048;
        taco_actor_rec[17].f132 = 20;
        taco_actor_rec[18].f1 = 64;
        taco_actor_rec[18].f120 = 2048;
        taco_actor_rec[18].f132 = 20;
        taco_actor_rec[19].f1 = 64;
        taco_actor_rec[19].f120 = 2048;
        taco_actor_rec[19].f132 = 20;
        taco_actor_rec[20].f1 = 64;
        taco_actor_rec[20].f120 = 2048;
        taco_actor_rec[20].f132 = 20;
        taco_actor_rec[21].f1 = 64;
        taco_actor_rec[21].f120 = 2048;
        taco_actor_rec[21].f132 = 20;
        taco_actor_rec[22].f1 = 64;
        taco_actor_rec[22].f120 = 2048;
        taco_actor_rec[22].f132 = 20;
        taco_actor_rec[23].f1 = 64;
        taco_actor_rec[23].f120 = 2048;
        taco_actor_rec[23].f132 = 20;
        taco_actor_rec[24].f1 = 64;
        taco_actor_rec[24].f120 = 2048;
        taco_actor_rec[24].f132 = 20;
        taco_actor_rec[25].f1 = 64;
        taco_actor_rec[25].f120 = 2048;
        taco_actor_rec[25].f132 = 20;
        taco_actor_rec[26].f1 = 64;
        taco_actor_rec[26].f120 = 2048;
        taco_actor_rec[26].f132 = 20;
    }

    if (t == 55) {
        taco_actor_rec[16].f1 = 128;
        taco_actor_rec[17].f1 = 128;
        taco_actor_rec[18].f1 = 128;
        taco_actor_rec[19].f1 = 128;
        taco_actor_rec[20].f1 = 128;
        taco_actor_rec[21].f1 = 128;
        taco_actor_rec[22].f1 = 128;
        taco_actor_rec[23].f1 = 128;
        taco_actor_rec[24].f1 = 128;
        taco_actor_rec[25].f1 = 128;
        taco_actor_rec[26].f1 = 128;
    }
}
