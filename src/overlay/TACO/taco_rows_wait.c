/* Address: 0x8014F1E0 */
/* Size: 144 bytes (36 insns) */
/* @SymbolSize: 144 */
#include "types.h"
#include "tokimeki/libgs_types.h"

typedef struct {
    u8 pad[118];
    s16 field_76;
} SomeStruct;

extern SomeStruct *taco_actor_rec;
extern LIBGS_COORD2 taco_coord[];

extern void sub_sub_step_up(void);
extern s32 abs(s32);
extern void taco_rec_move(s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void taco_rows_init(void);
extern void taco_anim_disp(s32);
extern void taco_anim(s32);

void taco_rows_wait(void) {
    s32 result;

    taco_anim(5);
    taco_anim_disp(7);
    taco_rec_move(0, 0, 1, 0, 0, 0, 0, 50, 500);
    result = abs(taco_coord[0].coord.t[1] - taco_actor_rec->field_76);
    if (result < 100) {
        taco_rows_init();
        sub_sub_step_up();
    }
}
