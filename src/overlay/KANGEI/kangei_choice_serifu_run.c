/* Address: 0x80136238 */
/* Size: 132 bytes (33 instructions) */
#include "types.h"

extern s32 step_counter;
extern s16 kangei_now_s_in;
extern u8 kangei_choice_no;
extern u8 kangei_choice_tbl;
extern u8 kangei_choice_rnd;

extern void func_80133F08(void);
extern void device_cmd_send(s32);

void kangei_choice_serifu_run(void) {
    s32 counter;
    s16 step;
    u8 idx;

    func_80133F08();
    counter = step_counter;
    {
        u32 v = (u32)counter < 1U;
        step_counter = counter + 1;
        if (v == 0) return;
    }
    step = kangei_now_s_in;
    if (step != 3) return;
    idx = kangei_choice_no;
    if ((&kangei_choice_tbl)[idx] != 0) return;
    if (kangei_choice_rnd != 0) return;
    device_cmd_send(1283);
}
