/* @SymbolSize: 160 */
/* @DataBase: 0x80134294 */
/* rensyu_main - RENSYU overlay, Address: 0x80132040, Size: 168 bytes (42 insns) */
#include "types.h"

extern u8  now_sub_step;

extern void rensyu_init_main(void);
extern void rensyu_girl_pick_main(void);
extern void rensyu_girl_talk_main(void);
extern void rensyu_load3(void);
extern void rensyu_trial_setup_main(void);
extern void rensyu_trial_run_main(void);
extern void rensyu_girl_reply_main(void);
extern void rensyu_result_apply_main(void);

void rensyu_main(void)
{
    u8 idx;
    void (*tbl[8])(void) = {
        rensyu_init_main, rensyu_girl_pick_main, rensyu_girl_talk_main, rensyu_load3,
        rensyu_trial_setup_main, rensyu_trial_run_main, rensyu_girl_reply_main, rensyu_result_apply_main,
    };

    idx = now_sub_step;
    tbl[idx]();
}
