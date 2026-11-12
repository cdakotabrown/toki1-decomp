/* @SymbolStart: 0x80140EB8 */
/* @SymbolSize: 172 */
/* @CompileEnv: IDO52_LDA_NOINREG_CLASSES=4 */
/* @PostProc: patch_insn_reg_field 0x28:rt:v1:v0 0x2c:rt:v0:a0 0x30:rs:v1:a0 0x30:rt:v1:a0 0x38:rt:a0:v1 0x44:rs:a0:v1 0x50:rt:v1:a0 */
/* name_ent_file_error_select - NAME_ENT overlay, Address: 0x80140EC0, Size: 164 bytes (41 insns) */
#include "types.h"

extern u8 album_work[];
extern u8 album_status_end;
extern u8 card_sys_present;

extern void sub_sub_step_up(void);
extern void goto_sub_step(s32);
extern void goto_sub_sub_step(s32);
extern s32 pad_trig;

void name_ent_file_error_select(void)
{
    u8 *p;
    u8 *end;

    if (pad_trig & 0x860) {
        end = &album_status_end;
        p = album_work;
        do {
            if (p[4] == 1) {
                break;
            }
            p++;
        } while (p != end);
        if (p == &album_status_end) {
            sub_sub_step_up();
        } else {
            goto_sub_step(2);
            if (card_sys_present != 0) {
                goto_sub_sub_step(1);
            } else {
                goto_sub_sub_step(32);
            }
        }
    }
}
