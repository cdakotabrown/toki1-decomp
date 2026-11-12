/* Address: 0x8007988C (recorded 0x80079894 is the PROLOGUE; the */
/* Size: 776 bytes (194 instructions; the .sym window runs to 0x80079BA0 */
/* Region: Game (IDO 5.2 -O2, -Wc,-pic1) */
/* @SymbolStart: 0x8007988C */
/* @SymbolSize: 776 */
/* @CompileEnv: IDO52_NO_CSE_OPCODES=cvt IDO52_PRECOLOR_WEB=v0:10 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 step_mode;
extern u8 card_result;
extern u32 card_cnt;
extern u8 last_album;

extern s32 card_step_init(void);
extern s32 card_check_main(void);
extern s32 cursor_work_copy(s32);
extern s32 card_state_clear(void);
extern s32 card_file_check(s32);
extern s32 card_file_read_main(void);
extern s32 save_work_restore(s32);
extern s32 card_file_write_main(void);
extern s32 device_cmd_send(s32);
extern s32 sub_step_up(void);

s32 ending_card_step(void)
{
    s32 r;

    switch ((u32)step_mode) {
    case 0:
        r = card_step_init();
        step_mode++;
        break;
    case 1:
        r = card_check_main();
        if (r != 0) {
            if (card_result == 1) {
                step_mode++;
                break;
            }
            if (card_result == 241) {
                step_mode++;
                break;
            }
            if (card_result == 255) {
                step_mode++;
                break;
            }
        } else {
            if (card_cnt >= 513) {
                step_mode++;
            }
        }
        break;
    case 2:
        cursor_work_copy(0);
        r = card_state_clear();
        if (card_result == 1) {
            r = card_file_check(0);
            if (r == 1) {
                step_mode++;
                r = card_step_init();
                return r;
            }
            step_mode = 255;
        } else {
            step_mode = 255;
        }
        break;
    case 3:
        r = card_file_read_main();
        if (r == 3) {
            r = card_state_clear();
            step_mode++;
        } else {
            if (card_cnt < 513) {
                if (card_result != 240) {
                    return r;
                }
            }
            r = card_state_clear();
            step_mode = 255;
        }
        break;
    case 4:
        r = save_work_restore(0);
        if (r == -1) {
            step_mode = 255;
        } else {
            step_mode++;
        }
        r = cursor_work_copy(1);
        return r;
    case 5:
        r = card_step_init();
        last_album = 0;
        step_mode++;
        break;
    case 6:
        r = card_file_write_main();
        if (r == 19) {
            device_cmd_send(1295);
            r = card_state_clear();
            step_mode++;
        } else {
            if (card_cnt < 513) {
                if (card_result != 240) {
                    return r;
                }
            }
            device_cmd_send(1282);
            r = card_state_clear();
            step_mode++;
        }
        break;
    case 255:
    default:
        r = sub_step_up();
        break;
    }
    return r;
}
