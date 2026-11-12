/* Address: 0x80136BAC */
/* Size: 104 bytes (26 insns) */
/* @SymbolSize: 104 */
/* @PostProc: patch_insn_reg_field 0x18:rt:t7:v0 0x24:rt:t8:t7 0x28:rs:t7:v0 0x28:rt:t7:v0 0x2c:rs:t8:t7 0x2c:rt:t8:t7 0x30:rt:a0:v1 0x34:rs:t7:v0 0x34:rt:t8:t7 0x38:rt:a0:v1 0x44:rs:a0:v1 */
#include "types.h"

extern u32 taiiku_kari_run_0_speed_cnt;
extern s32 taiiku_kari_run_0_speed;
extern s32 taiiku_kari_run_0_accel;
extern volatile s32 taiiku_kari_vmax;

void taiiku_speed_set(void) {
    s32 speed;
    s32 floor;

    if (taiiku_kari_run_0_speed_cnt < 11) {
        return;
    }

    speed = taiiku_kari_run_0_speed + taiiku_kari_run_0_accel;
    if (speed < -0x10000) {
        speed = -0x10000;
    }

    floor = taiiku_kari_vmax;
    taiiku_kari_run_0_speed = speed;
    if (floor < speed) {
        taiiku_kari_run_0_speed = floor;
    }
    taiiku_kari_run_0_speed_cnt = 0;
}
