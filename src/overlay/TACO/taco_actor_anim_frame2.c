/* @CompileFlags: -cckr */
/* @SymbolSize: 304 */
/* Address: 0x8015BF80 .. 0x8015C218  (664 bytes / 166 insns) */
#include "types.h"

typedef struct {
    u8  f0;
    u8  f1;
    u8  pad_2[2];
    u32 f4;
    u8  pad_8[122];
    s16 f130;
    u8  pad_132[2];
    u8  f134;
    u8  pad_135[1];
} Entry;

typedef struct {
    u32 a;
    u32 b;
} Pair;

typedef struct {
    u32 x;
    u8  pad[12];
} Slot16;

extern u8 *taco_actor_rec;

#define ENT(n) ((Entry *)(taco_actor_rec + (n) * 136))
extern u8 taco_node_own[];
extern u8 taco_kind_node_tbl[];
extern u32 *taco_kind_pose_tbl[];
extern Slot16 taco_obj[];

taco_actor_anim_frame2(id)
{
    s32 i;
    s32 k;

    if (ENT(id)->f130 >= 15) {
        ENT(id)->f130 = 0;
    }
    k = 0;
    for (i = 0; i < 64; i++) {
        if (id == taco_node_own[i]) {
            taco_rec_matrix_set(i, taco_kind_pose_tbl[ENT(id)->f134][k * 2],
                          taco_kind_pose_tbl[ENT(id)->f134][k * 2 + 1], ENT(id)->f130);
            k++;
        }
        if (k >= taco_kind_node_tbl[ENT(id)->f134]) {
            break;
        }
    }
}
