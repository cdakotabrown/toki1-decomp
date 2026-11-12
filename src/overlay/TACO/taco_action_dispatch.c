/* @SymbolStart: 0x8014D2B0 */
/* @RodataBase: 0x8015DC60 */
/* @SymbolSize: 212 */
/* /* @PostProc: patch_insn_reg_field 0x20:rt:v0:t7 0x28:rs:v0:t7 0x30:rt:v0:t7 */
/* Address: 0x8014D2B8 */
#include "types.h"

extern u8 * taco_actor_rec;

extern void taco_action_mode0_sweep_step(s32);
extern void taco_action_mode1_rec_move_step(void);
extern void taco_action_mode2_phase_pan_step(void);
extern void taco_action_mode3_rec_burst_step(void);
extern void taco_action_mode5_run_step(void);
extern void taco_action_mode6_stage_run_step(void);
extern void taco_action_mode7_band_shot_step(void);
extern void taco_action_mode8_cut_run_step(void);

void taco_action_dispatch(void)
{
    u8 *t6_in = taco_actor_rec;

    u8 sel = t6_in[2310];

    switch (sel) {
        case 0: taco_action_mode0_sweep_step(1); break;
        case 1: taco_action_mode1_rec_move_step(); break;
        case 2: taco_action_mode2_phase_pan_step(); break;
        case 3: taco_action_mode3_rec_burst_step(); break;
        case 4: taco_action_mode0_sweep_step(-1); break;
        case 5: taco_action_mode5_run_step(); break;
        case 6: taco_action_mode6_stage_run_step(); break;
        case 7: taco_action_mode7_band_shot_step(); break;
        case 8: taco_action_mode8_cut_run_step(); break;
    }
}
