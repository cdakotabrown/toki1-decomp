/* Address: 0x8013B960 */
/* Size: 120 bytes (30 insns) */
/* @SymbolSize: 120 */
#include "types.h"

extern u8 taco_effect_buf[];
extern u8 taco_actor_buf[];
extern u8 taco_effect2_buf[];
extern u8 taco_actor2_buf[];

extern void taco_obj_hit(void*, s32, s32);
extern void taco_pickup_hit(void);
extern void taco_player_hit(void);

void taco_hit_all(void) {
    taco_obj_hit(taco_effect_buf, 1, 0);
    taco_obj_hit(taco_actor_buf, 4, 0);
    taco_obj_hit(taco_effect2_buf, 1, 1);
    taco_obj_hit(taco_actor2_buf, 4, 1);
    taco_pickup_hit();
    taco_player_hit();
}
