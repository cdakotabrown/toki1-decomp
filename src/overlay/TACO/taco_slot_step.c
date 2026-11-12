/* @CompileEnv: IDO52_FORCE_WEB_COLOR=8:s1,12:s0 */
/* @PostProc: patch_insn_reg_field 0x164:rt:t4:t5 0x16c:rs:t4:t5 0x16c:rt:t5:t6 0x170:rt:t5:t6 */
/* Address: 0x80151F64 */
/* Size: 380 bytes (95 insns) */
#include "types.h"

extern u8 *taco_actor_rec;

extern void sd_cmd_limit_send(s32, s32);
extern void taco_node_shot_spawn(s32, s32, s32, s32, s32, s32, s32);
extern void taco_glyph_row_disp(s32, s32, s32);
extern void taco_rows_disp(s32, s32);
extern void taco_reel_result_set(s32);

void taco_slot_step(u8 *timer, u8 *step, s32 kind, s32 arg3, s32 arg4)
{
    u8 unused[60];
    s32 idx;

    if ((u32)arg4 < (u32)*timer) {
        taco_glyph_row_disp(kind, arg3, *step);
        *step = *step + 1;
        *timer = 0;
    }

    if ((u32)*step >= 30U && (u32)*step < 33U) {
        if (kind == 1) {
            idx = 16;
        } else if (kind == 2) {
            idx = 17;
        } else if (kind == 3) {
            idx = 18;
        }
        if (taco_actor_rec[idx * 136 + 133] != 0) {
            taco_node_shot_spawn(0, kind, 0, 100, 1000, 200, 131);
            if (*timer == 0 && *step == 30) {
                sd_cmd_limit_send(0, 1281);
            }
        }
    }

    if ((u32)*step >= 40U) {
        *step = 0;
        *timer = 0;
        taco_reel_result_set(kind);
        taco_rows_disp(kind, arg3);
    } else {
        *timer = *timer + 1;
    }
}
