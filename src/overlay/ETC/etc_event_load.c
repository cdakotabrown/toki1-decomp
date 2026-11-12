/* @CompileEnv: IDO52_AS1_NO_LA_PULL=5,6 IDO52_AS1_NO_FALLTHRU_FILL=all */
/* @PostProc: patch_stack_complete --old 24 --new 40 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x88 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:a1 0x14:rs:v0:a1 0x14:rt:v0:a1 0x24:rs:v0:a1 0x2c:rs:v0:a1 0x34:rs:v0:a1 0x3c:rs:v0:a1 0x60:rt:v0:a1 0x70:rt:v0:a1 0x74:rs:v0:a1 0x74:rt:v0:a1 0x7c:rs:v0:a1 0x7c:rt:v0:a1 0x84:rt:v0:a1 0x90:rt:v0:a1 0x178:rt:v0:a1 0x17c:rt:v0:a1 0x180:rs:v0:a1 0x180:rt:v0:a1 0x188:rs:v0:a1 0x188:rt:v0:a1 0x190:rt:v0:a1 0x19c:rt:v0:a1 0x210:rt:v0:a1 0x214:rs:v0:a1 0x214:rt:v0:a1 0x21c:rs:v0:a1 0x21c:rt:v0:a1 0x220:rt:v0:a1 */
/* @CompileFlags: -Wc,-pic0 */
/* @SymbolStart: 0x8013E88C */
/* @SymbolSize: 548 */
/* Address: 0x8013E894 (window starts 0x8013E88C - absorbed cross-prologue head) */
/* Size: 548 bytes (137 instructions, head included) */
#include "types.h"

extern s32 get_CD_State2(void);
extern void dec_bg_reset(void);
extern void etc_menu_dispatch(s32, s32, s32, s32);
extern void etc_hizuke_set(void);
extern s32 etc_event_seek(void);
extern void etc_event_read(s32);
extern void etc_girl_record_search(void);

extern s32 etc_bg_req;
extern s32 etc_bustup_flag;
extern s32 etc_rec_kind;
extern s32 etc_rec_id;
extern s32 etc_rec_girl;
extern s32 etc_rec_season;
extern s32 etc_record_index;
extern s32 etc_event_step;
extern s32 etc_event_flag;
extern s32 etc_call_choice;

void etc_event_load(s32 arg0)
{
    switch (etc_event_step) {
    case -1:
        break;

    case 0:
        etc_hizuke_set();
        if (etc_event_seek() != 0) {
            etc_event_read(arg0);
            etc_event_step = etc_event_step + 1;
        } else {
            etc_event_step = -1;
        }
        break;

    case 1:
        if (etc_bustup_flag == 1 || etc_event_flag == 1) {
            if ((get_CD_State2() & 1) == 0) {
                return;
            }
            dec_bg_reset();
            etc_menu_dispatch(etc_rec_kind, etc_rec_id, etc_rec_girl, etc_rec_season);

            if (etc_bg_req == 0 && etc_bustup_flag == 1) {
                if (etc_record_index < 255) {
                    etc_record_index = etc_record_index + 1;
                }
                if (etc_call_choice == 0 || etc_call_choice == 2) {
                    etc_girl_record_search();
                }
                etc_event_step = 0;
            } else if (etc_bg_req == 1) {
                etc_event_step = etc_event_step + 1;
            } else {
                etc_event_step = 3;
            }
        } else {
            if (etc_bustup_flag != 0) {
                return;
            }
            if (etc_event_flag != 0) {
                return;
            }
            if (etc_record_index < 255) {
                etc_record_index = etc_record_index + 1;
            }
            if (etc_call_choice == 0 || etc_call_choice == 2) {
                etc_girl_record_search();
            }
            etc_event_step = 0;
        }
        break;

    case 2:
        if ((get_CD_State2() & 1) != 0) {
            etc_event_step = etc_event_step + 1;
        }
        break;
    }
}
