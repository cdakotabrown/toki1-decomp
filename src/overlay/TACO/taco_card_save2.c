/* @SymbolStart: 0x801429C0 */
/* @SymbolSize: 752 */
/* @RodataBase: 0x8015DAB0 */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:3:deny,lod:4:deny,lod:5:deny IDO52_FORCE_WEB_COLOR=2:v1,14:v1,18:v1 */
/* Address: 0x801429C0 (cross-prologue head) / 0x801429C8 (prologue) */
/* Size: 752 bytes (188 insns incl. the 2-insn head) */
#include "types.h"

extern u8 step_mode;
extern u8 card_result;
extern u32 card_cnt;
extern u8 last_album;

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);
extern void card_step_init(void);
extern void card_state_clear(void);
extern s32 card_check_main(void);
extern s32 card_file_read_main(void);
extern s32 card_file_write_main(void);
extern s32 card_file_check(s32);
extern s32 save_work_restore(s32);
extern void cursor_work_copy(s32);

void taco_card_save2(void)
{
    s32 f;

    switch ((u32)step_mode) {
    case 0:
        if (card_check_main() != 0) {
            f = card_result;
            if (f == 1) {
                step_mode = step_mode + 1;
            } else if (f == 241) {
                step_mode = step_mode + 1;
            } else if (f == 255) {
                step_mode = step_mode + 1;
            }
        } else if (card_cnt >= 513) {
            step_mode = step_mode + 1;
        }
        break;
    case 1:
        card_state_clear();
        f = card_result;
        if (f == 1) {
            if (card_file_check(0) == 1) {
                step_mode = step_mode + 1;
                card_step_init();
                cursor_work_copy(0);
            } else {
                step_mode = 255;
            }
        } else {
            step_mode = 255;
        }
        break;
    case 2:
        if (card_file_read_main() == 3) {
            card_state_clear();
            step_mode = step_mode + 1;
        } else if (card_cnt >= 513 || (f = card_result) == 240) {
            card_state_clear();
            step_mode = 255;
        }
        break;
    case 3:
        if (save_work_restore(0) == -1) {
            cursor_work_copy(1);
            step_mode = 255;
        } else {
            cursor_work_copy(1);
            step_mode = step_mode + 1;
        }
        break;
    case 4:
        card_step_init();
        last_album = 0;
        step_mode = step_mode + 1;
        break;
    case 5:
        if (card_file_write_main() == 19) {
            device_cmd_send(1295);
            card_state_clear();
            step_mode = step_mode + 1;
        } else if (card_cnt >= 513 || (f = card_result) == 240) {
            device_cmd_send(1282);
            card_state_clear();
            step_mode = step_mode + 1;
        }
        break;
    case 255:
    default:
        sub_sub_step_up();
        break;
    }
}
