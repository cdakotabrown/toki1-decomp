/* @SymbolStart: 0x8013547C */
/* @SymbolSize: 240 */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=1 IDO52_UGEN_TEMP_REG=3:t9,4:t0,5:t1,6:t2,7:t3,8:t4,9:t5,10:t6,11:t7 */
#include "types.h"

extern volatile s8 taco_menu_csr;
extern s32 pad_trig;
extern u32 step_state;

extern void sd_cmd_limit_send(s32, s32);
extern void sub_sub_step_up(void);
extern void goto_sub_step(s32);
extern void taco_menu_str_disp(void);

void taco_select_adjust(void)
{
    s32 pad;
    s32 v;

    pad = pad_trig;
    if (pad & 0x5100) {
        if (pad & 0x1000) {
            taco_menu_csr--;
        }
        v = taco_menu_csr;
        if (pad & 0x4000) {
            v = (s8)(v + 1);
        }
        if (pad & 0x100) {
            v = (s8)(v + 1);
        }
        if (v < 0) {
            v = 4;
        }
        taco_menu_csr = v;
        if (v >= 5) {
            taco_menu_csr = 0;
        }
        sd_cmd_limit_send(0, 1280);
        pad = pad_trig;
    }
    if (pad & 0x820) {
        sd_cmd_limit_send(0, 1281);
        sub_sub_step_up();
    }
    taco_menu_str_disp();
    if (step_state >= 901) {
        goto_sub_step(6);
    }
}
