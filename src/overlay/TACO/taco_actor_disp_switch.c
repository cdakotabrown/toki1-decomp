/* @CompileFlags: -cckr */
/* @SymbolStart: 0x8015C0B0 */
/* @SymbolSize: 360 */
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

taco_actor_disp_switch(a, id)
{
    s32 i;

    if (ENT(id)->f1 & 0x80) {
        if (a == 1) {
            if (ENT(id)->f134 < 11) {
                ENT(id)->f4 = 0;
                return;
            }
            ENT(id)->f4 = 0x80000000;
            for (i = 0; i < 64; i++) {
                if (id == taco_node_own[i]) {
                    taco_obj[i].x = 0;
                }
            }
        } else {
            if (ENT(id)->f134 < 9) {
                ENT(id)->f4 = 0x80000000;
                return;
            }
            ENT(id)->f4 = 0x80000000;
            for (i = 0; i < 64; i++) {
                if (id == taco_node_own[i]) {
                    taco_obj[i].x = 0x80000000;
                }
            }
        }
    }
}
