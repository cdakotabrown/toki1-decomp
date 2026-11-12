/* Function: holiday_tel_exit */
/* Address: 0x800781DC  -> window now starts at 0x800781D4 */
/* Size: 416 bytes (104 instructions)  -> window 424 bytes / 106 insns */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileEnv: IDO52_PRECOLOR_WEB=a0:0,a0:2,v1:23 */
/* @SymbolStart: 0x800781D4 */
/* @SymbolSize: 424 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: patch_insn_reg_field 0x54:rt:a2:v1 0x58:rs:a2:v1 0xe0:rt:a2:v1 0xe4:rs:a2:v1 0x168:rt:a2:v1 0x16c:rs:a2:v1 0x16c:rt:a2:v1 0x174:rs:a2:v1 0x184:rt:a2:v1 */
#include "common.h"

extern s32 step_counter;
extern u8  step_mode;
extern u8  now_girl;
extern s8  now_icon;
extern s32 work_fuku;

extern u8  holiday_tel_exit_tbl1[];
extern u8  holiday_tel_exit_tbl0[][7];

extern void device_cmd_send(s32 arg0);
extern void k_sub_reset(void);
extern void restore_bgm(void);
extern void goto_step(s32 arg0);
extern void goto_sub_step(s32 arg0);

void holiday_tel_exit(void) {
    s32 v0;
    u8 v1;

    if (step_mode != 11) {
        if ((step_counter & 7) == 7) {
            if (step_mode == 0) {
                device_cmd_send(1284);
            } else if ((u32)step_mode >= 4U) {
                v0 = step_mode - 4;
                v1 = now_girl;
                if (v1 == 13) {
                    device_cmd_send((u16)(holiday_tel_exit_tbl1[v0] + 1285));
                } else {
                    device_cmd_send((u16)(holiday_tel_exit_tbl0[v1][v0] + 1285));
                }
            }
            step_mode = (u8)(step_mode + 1);
        }
    } else {
        v1 = now_girl;
        if (v1 == 13) {
            now_icon = 0;
            k_sub_reset();
            restore_bgm();
            goto_step(69);
            goto_sub_step(0);
            return;
        }
        if (v1 == 11) {
            k_sub_reset();
            now_icon = 12;
            work_fuku = 11;
            restore_bgm();
            goto_step(69);
            goto_sub_step(1);
            return;
        }
        k_sub_reset();
        v1 = now_girl;
        now_icon = (s8)(v1 + 1);
        work_fuku = v1;
        restore_bgm();
        goto_step(69);
        goto_sub_step(1);
    }
}
