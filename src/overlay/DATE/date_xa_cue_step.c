/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
/* @SymbolSize: 520 */
/* Function: date_xa_cue_step */
/* Overlay: DATE */
/* @PostProc: patch_insn_reg_field 0x124:rd:a0:v0 0x128:rt:a2:a0 0x12c:rs:a0:v0 0x12c:rt:a2:a0 0x138:rt:v1:t3 0x13c:rs:v1:t3 0x13c:rt:v1:t3 0x140:rs:a0:v0 0x140:rt:a0:v0 0x144:rd:t9:t8 0x144:rt:a0:v0 0x148:rd:a0:v0 0x148:rt:t9:t8 0x154:rd:v0:v1 0x158:rs:v0:v1 0x158:rt:t3:t4 0x160:rs:v1:t3 0x160:rt:t3:t4 0x168:rs:a0:v0 0x16c:rs:v0:v1 0x16c:rt:t4:t5 0x174:rs:t4:t5 0x174:rt:v1:t3 0x17c:rs:a0:v0 0x180:rs:v0:v1 0x180:rt:t5:t9 0x18c:rt:t5:t9 0x190:rs:a0:v0 0x1a0:rt:t6:t7 0x1a8:rt:t6:t7 0x1ac:rt:t7:t1 0x1b0:rs:t7:t1 0x1b0:rt:t7:t1 0x1b4:rt:t1:t0 0x1b8:rs:t7:t1 0x1c0:rt:t0:t2 0x1cc:rs:t1:t0 0x1cc:rt:t1:t0 0x1d4:rs:t1:t0 0x1e0:rt:t0:t2 0x1ec:rt:t2:t4 0x1f0:rs:t2:t4 0x1f4:rt:t3:t5 0x1f8:rs:t3:t5 0x1f8:rt:t3:t5 0x200:rs:t3:t5 0x200:rt:t4:t3 0x204:rt:t4:t3 */
#include "types.h"

typedef struct {
    s32 x;
    s32 y;
} DateVec2;

extern DateVec2 date_xa_cue_step_tbl;
extern s16 *date_xa_cue_tbl;
extern volatile u32 step_counter;
extern volatile u32 pad_trig;
extern u8 cw97_mode;
extern s16 cal_cw_frame;

extern void sub_sub_step_up();
extern void device_cmd_send();
extern s32 get_CD_l_com();
extern s32 get_CD_State();
extern void set_XAsector();

s32 date_xa_cue_step(void)
{
    s32 found;
    s16 i;
    u32 now;
    DateVec2 pos;

    pos = date_xa_cue_step_tbl;

    if (step_counter == 0) {
        cw97_mode = 1;
        set_XAsector(pos.x, pos.y, 15);
        device_cmd_send(191);
        device_cmd_send(768);
        step_counter = step_counter + 1;
    } else {
        if (get_CD_l_com() == 9) {
            if (step_counter >= 61) {
                cw97_mode = 0;
                sub_sub_step_up();
                device_cmd_send(514);
                return 0;
            }
        }

        if ((pad_trig & 0x400040) != 0) {
            if (step_counter >= 61) {
                cw97_mode = 0;
                sub_sub_step_up();
                cal_cw_frame = 3;
                device_cmd_send(180);
                device_cmd_send(514);
                return 0;
            }
        }

        found = 0;
        for (i = 0; i < 167; i++) {
            now = step_counter;
            if (now < date_xa_cue_tbl[i * 3]) {
                continue;
            }
            if (date_xa_cue_tbl[i * 3 + 1] < now) {
                continue;
            }
            cal_cw_frame = date_xa_cue_tbl[i * 3 + 2];
            found = 1;
        }
        if (found == 0) {
            cal_cw_frame = 3;
        }
    }

    if (step_counter >= 1660) {
        cw97_mode = 0;
    }
    if (step_counter < 6371) {
        cw97_mode = 1;
    }
    if ((get_CD_State() & 4) != 0) {
        step_counter = step_counter + 1;
    }
}
