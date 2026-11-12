/* @SymbolStart: 0x801358C8 */
/* @SymbolSize: 296 */
/* @RodataBase: 0x8013BDEC */
/* master_serifu15 - MASTER overlay, Address: 0x801358D0, Size: 288 bytes (72 insns) */
#include "types.h"

extern void cd_read_start2();
extern void sub_sub_step_up(void);
extern s32 get_CD_State2(void);
extern void master_serifu_set3(void);
extern void serifu_run(void);
extern void sub_step_up(void);
extern void game_reset(void);
extern void common_disp(void);

extern s32 master_serifu_ptr[];
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern u8 now_sub_sub_step;

void master_serifu15(void) {
    s32 hidden_t6;

    hidden_t6 = now_sub_sub_step;
    switch (hidden_t6) {
    case 0:
        cd_read_start2(11, (void *)0x801E8000, 0xA9C5);
        sub_sub_step_up();
        break;
    case 1:
        if (get_CD_State2() & 1) {
            master_serifu_set3();
            ss_serifu = master_serifu_ptr[0];
            ss_serifu_kind = master_serifu_ptr[1];
            sss_sound = master_serifu_ptr[2];
            now_serifu = 0;
            now_serifu_in = 0;
            sub_sub_step_up();
        }
        break;
    case 2:
        serifu_run();
        break;
    case 3:
        now_serifu = 5;
        sub_sub_step_up();
        break;
    case 4:
        serifu_run();
        break;
    case 5:
        sub_step_up();
        break;
    default:
        game_reset();
        break;
    }
    common_disp();
}
