/* @SymbolStart: 0x801354C8 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8  pad0[2];
    u8  f2;
    u8  f3;
    u8  pad4[3];
    u8  f7;
    u8  pad8[59];
    u8  f67;
} Rec;

extern Rec class_w[];
extern u32 step_counter;
extern u8  scene_entry_b;

bunka_sd_slot_ani()
{
    if (step_counter == 0) {
        scene_entry_b = 128;
        class_w[96].f3 = 0;
        class_w[98].f3 = 0;
        class_w[97].f7 = 128;
        class_w[97].f3 = 128;
        class_w[97].f2 = 5;
    }
    basyo_show();
    step_counter++;
    if (step_counter % 9 == 0) {
        class_w[97].f67++;
        if (class_w[97].f67 >= 21) {
            class_w[97].f67 = 17;
        }
    }
    if (!(class_w[97].f2 & 1)) {
        sub_sub_step_up();
    }
}
