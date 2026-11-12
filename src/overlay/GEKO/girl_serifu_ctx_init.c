/* @SymbolStart: 0x801355A4 */
/* @SymbolSize: 684 */
/* @RodataBase: 0x801447A4 */
/* @CompileFlags: -cckr */
#include "types.h"
extern s16 geko_now_serifu2;
extern s16 geko_now_serifu2_in;
typedef struct {
    u8 pad0[0x71E];
    u8 flags;
    u8 pad1[0xF5F - 0x71F];
    u8 sel;
} Save;
extern Save screen_w;

extern void sub_sub_step_up(void);

extern s32 geko_serifu1_tbl[];
extern s32 geko_serifu1_kind_tbl[];
extern s32 geko_sound1_tbl[];
extern s32 geko_ss_serifu2;
extern s32 geko_ss_serifu2_kind;
extern s32 geko_sss_sound2;

void girl_serifu_ctx_init()
{
    screen_w.flags |= 4;
    screen_w.flags |= 0x40;
    geko_now_serifu2 = 1;
    geko_now_serifu2_in = 0;

    switch (screen_w.sel) {
    case 0:
        geko_ss_serifu2 = geko_serifu1_tbl[1];
        geko_ss_serifu2_kind = geko_serifu1_kind_tbl[1];
        geko_sss_sound2 = geko_sound1_tbl[1];
        break;
    case 1:
        geko_ss_serifu2 = geko_serifu1_tbl[6];
        geko_ss_serifu2_kind = geko_serifu1_kind_tbl[6];
        geko_sss_sound2 = geko_sound1_tbl[6];
        break;
    case 2:
        geko_ss_serifu2 = geko_serifu1_tbl[3];
        geko_ss_serifu2_kind = geko_serifu1_kind_tbl[3];
        geko_sss_sound2 = geko_sound1_tbl[3];
        break;
    case 3:
        geko_ss_serifu2 = geko_serifu1_tbl[4];
        geko_ss_serifu2_kind = geko_serifu1_kind_tbl[4];
        geko_sss_sound2 = geko_sound1_tbl[4];
        break;
    case 4:
        geko_ss_serifu2 = geko_serifu1_tbl[10];
        geko_ss_serifu2_kind = geko_serifu1_kind_tbl[10];
        geko_sss_sound2 = geko_sound1_tbl[10];
        break;
    case 5:
        geko_ss_serifu2 = geko_serifu1_tbl[7];
        geko_ss_serifu2_kind = geko_serifu1_kind_tbl[7];
        geko_sss_sound2 = geko_sound1_tbl[7];
        break;
    case 6:
        geko_ss_serifu2 = geko_serifu1_tbl[8];
        geko_ss_serifu2_kind = geko_serifu1_kind_tbl[8];
        geko_sss_sound2 = geko_sound1_tbl[8];
        break;
    case 7:
        geko_ss_serifu2 = geko_serifu1_tbl[5];
        geko_ss_serifu2_kind = geko_serifu1_kind_tbl[5];
        geko_sss_sound2 = geko_sound1_tbl[5];
        break;
    case 8:
        geko_ss_serifu2 = geko_serifu1_tbl[0];
        geko_ss_serifu2_kind = geko_serifu1_kind_tbl[0];
        geko_sss_sound2 = geko_sound1_tbl[0];
        break;
    case 9:
        geko_ss_serifu2 = geko_serifu1_tbl[9];
        geko_ss_serifu2_kind = geko_serifu1_kind_tbl[9];
        geko_sss_sound2 = geko_sound1_tbl[9];
        break;
    case 10:
        geko_ss_serifu2 = geko_serifu1_tbl[12];
        geko_ss_serifu2_kind = geko_serifu1_kind_tbl[12];
        geko_sss_sound2 = geko_sound1_tbl[12];
        break;
    }
    sub_sub_step_up();
}
