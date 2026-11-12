/* @SymbolStart: 0x8013BD50 */
/* Address: 0x8013BD50 (cross-prologue head) / 0x8013BD58 (body) */
/* Size: 228 bytes (57 insns) */
/* @SymbolSize: 228 */
/* @PostProc: patch_insn_reg_field 0x70:rs:v0:v1 0x70:rt:v1:v0 0x98:rs:v0:v1 0x98:rt:v1:v0 */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 bustup_face_step4(void);
extern s32 bustup_face_step3(void);

extern s32 serifu_no;
extern u8  now_girl;
extern u8  girl5_spot4_cnt;
extern s16 now_serifu;

s32 date_basyo2_face_step03(void) {
    s32 v0;

    if (serifu_no == 2) {
        v0 = now_girl;
        if (v0 == 2U || v0 == 3U) {
            return sub_sub_step_up();
        }
        if (v0 == 8) {
            return bustup_face_step4();
        }
        if (v0 == 5 && girl5_spot4_cnt == 0) {
            return sub_sub_step_up();
        }
        if (v0 == 5 && girl5_spot4_cnt != 0) {
            now_serifu = now_serifu + 1;
            return bustup_face_step3();
        }
        return bustup_face_step3();
    }
    return sub_sub_step_up();
}
