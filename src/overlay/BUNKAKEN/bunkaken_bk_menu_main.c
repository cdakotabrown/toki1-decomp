/* @SymbolStart: 0x80144C7C */
/* @SymbolSize: 220 */
/* @RodataBase: 0x80154B18 */
/* @PostProc: swap_registers v1:v0 */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x80 */
/* bunkaken_bk_menu_main - BUNKAKEN overlay, Address: 0x80144C84, Size: 212 bytes (53 insns) */
#include "types.h"

extern void sub_step_up(void);
extern s32 sub_sub_step_up(void);
extern s32 game_reset(void);
extern s32 serifu_run(void);
extern void bunkaken_menu_init(void);
extern void bunkaken_menu(void);
extern u8  now_sub_sub_step;
extern s16 now_serifu;
extern s32 serifu_no;
extern s32 bunka_bukatu;
extern s32 bunka_girl;

void
bunkaken_bk_menu_main(void)
{
    s32 t6 = now_sub_sub_step;
    s32 v0;

    switch ((u32)t6) {
    case 0:
        bunkaken_menu_init();
        sub_sub_step_up();
        break;
    case 1:
        bunkaken_menu();
        break;
    case 2:
        v0 = serifu_no;
        bunka_bukatu = v0;
        if (bunka_girl == 255) {
            now_serifu = v0 + 12;
            sub_sub_step_up();
        }
        else {
            sub_step_up();
        }
        break;
    case 3:
        serifu_run();
        break;
    case 4:
        sub_step_up();
        break;
    default:
        game_reset();
        break;
    }
}
