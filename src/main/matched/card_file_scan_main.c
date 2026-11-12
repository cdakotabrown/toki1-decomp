/* Function: card_file_scan_main */
/* Address: 0x80054530  (true start; 0x80054544 is the PROLOGUE) */
/* Size: 324 bytes (81 instructions) over the corrected window */
/* @SymbolStart: 0x80054530 */
/* @SymbolSize: 324 */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic1 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=9:v0 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=6 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 card_file_check(s32 arg);
extern void card_step_next(void);

struct Glob {
    u8 pad0;
    u8 f_E795;
    u8 pad1[0x10EA];
    s32 f_F880;
};
extern struct Glob sd_proc_on;

extern volatile u8 card_step;
extern u8 card_result;
extern u8 now_album;
extern u8 card_event_cnt;
extern u8 album_status[];
extern u8 card_sys_present;

u8 card_file_scan_main(void)
{
    s32 result;
    s32 state;
    s32 t0;

    sd_proc_on.f_F880++;
    if (sd_proc_on.f_E795 != 0) {
        state = card_step;
        if (state == 0) {
            card_result = 0;
            card_step = state;
            now_album = state;
            result = card_file_check(state);
            if (result == 1) {
                card_sys_present = 1;
                card_step_next();
                goto done;
            }
            t0 = (u8)(card_event_cnt + 1);
            card_event_cnt = t0;
            if ((u32)t0 < 3) {
                goto done;
            }
            card_step_next();
            goto done;
        }

        if ((u32)state < 15) {
            card_step = state;
            now_album = (u8)(state & 0xFF);
            result = card_file_check(now_album);
            if (result == 1) {
                album_status[now_album] = 1;
            } else {
                album_status[now_album] = 0;
            }
            card_step++;
        } else {
            sd_proc_on.f_E795 = 0;
            card_result = 2;
        }
    }

done:
    return card_result;
}
