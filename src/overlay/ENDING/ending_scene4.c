/* @PostProc: patch_stack_complete --old 80 --new 96 --shift-ra */
/* @PostProc: remap_stack_slots --from 44 --to 56 */
/* @PostProc: swap_registers v0:v1 --bidirectional */
/* @PostProc: rename_register_chain --chain v1:t9 */
/* @PostProc: rename_register_chain --chain t9:t6 --exclude-offsets 0x24,0x48,0x64,0x6c */
/* @PostProc: rename_register_chain --chain t0:t1 --exclude-offsets 0x18,0x24,0x28,0x2c,0x30,0x34,0x38,0x3c,0x40,0x4c */
/* @PostProc: patch_insn_reg_field 0xd8:rd:t6:t0 0xdc:rs:t6:t0 */
/* ending_scene4 - ENDING overlay, Address: 0x80135DF0, Size: 252 bytes (63 instructions) */
#include "types.h"

typedef struct {
    s32 data[9];
} DispatchTable9;

extern DispatchTable9 ending_scene4_tbl;
extern u8 now_sub_sub_step;
extern u8 ending_scene4_on;
extern u32 frame_counter;
extern s32 wink2_stop;

extern void grad_box_disp(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);
extern void dtd_on(s32 a0);
extern void ending_item_ani(void);
extern void ending_flag_set(void);

void ending_scene4(void)
{
    DispatchTable9 tbl;
    void (*fn)(s32);

    tbl = ending_scene4_tbl;

    fn = (void (*)(s32))tbl.data[now_sub_sub_step];
    fn(128);

    if (ending_scene4_on != 0) {
        grad_box_disp(-160, -120, 320, 240, 14, (s32)0x00C0C0FFU,
                      (s32)0x004080FFU);
        dtd_on(14);
        ending_item_ani();
    }

    if ((frame_counter % 240) != 0) {
        return;
    }
    if (wink2_stop != 0) {
        return;
    }
    ending_flag_set();
}
