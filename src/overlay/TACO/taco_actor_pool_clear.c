/* @PostProc: swap_instructions --offset1 0x98 --offset2 0xa0 */
/* Address: 0x8015AC00 */
/* Size: 220 bytes (55 insns) */
#include "types.h"

extern u8 *taco_actor_rec;

typedef struct {
    s32 f0;
    s32 pad[3];
} Ent16;

extern Ent16 taco_obj[64];
extern u8 taco_node_own[64];

extern s32 taco_phase_cnt;
extern s32 taco_phase_no;
extern s32 taco_phase_frame;
extern s32 taco_substep_no;
extern s32 taco_emit_mode;

extern void taco_object_slot_init(s32 idx);

void taco_actor_pool_clear(void)
{
    s32 fill;
    s32 i;
    s32 ofs;

    fill = (s32)0x80000000;
    for (i = 0; i < 64; i++) {
        taco_node_own[i] = 0;
        taco_obj[i].f0 = fill;
    }

    i = 16;
    ofs = 2176;
    do {
        taco_object_slot_init(i);
        *(s32 *)(taco_actor_rec + ofs + 4) = fill;
        *(s8 *)(taco_actor_rec + ofs + 134) = 1;
        i++;
        ofs += 136;
    } while (i != 63);

    taco_phase_cnt = 0;
    taco_phase_no = 0;
    taco_phase_frame = 0;
    taco_substep_no = 0;
    taco_emit_mode = 0;
}
