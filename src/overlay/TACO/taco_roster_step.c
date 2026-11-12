/* Address: 0x8014B274 */
/* Size: 208 bytes (52 insns) */
#include "types.h"

typedef struct {
    u8 pad[2310];
    u8 field_906;
    u8 pad2[3];
    u8 field_90A;
    u8 pad3[131];
    u8 field_98E;
} SomeStruct;

extern SomeStruct *taco_actor_rec;
extern u8 taco_main2_hp_tbl;

extern s32 get_taco_percent();
extern void taco_roster_up();
extern void taco_roster_down();
extern s32 taco_action_dispatch();

void taco_roster_step(s32 arg0)
{
    SomeStruct *ptr;
    s32 status;
    s32 one;

    taco_action_dispatch();
    ptr = taco_actor_rec;
    status = ptr->field_906;

    if (status == 0 || status == 2 || status == 3 || status == 4) {
        get_taco_percent(11, &taco_main2_hp_tbl);
        ptr = taco_actor_rec;
        one = 1;
        if (ptr->field_90A == one && arg0 >= 601) {
            ptr->field_98E = one;
        }
    }

    ptr = taco_actor_rec;
    if (ptr->field_98E == 1) {
        taco_roster_up(2);
        ptr = taco_actor_rec;
    }

    if (ptr->field_98E == 2) {
        taco_roster_down(2);
    }
}
