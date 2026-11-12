/* @SymbolSize: 344 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x5c */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x5c */
/* Address: 0x8013AF04 */
/* Size: 344 bytes (86 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

struct { s16 unk0; s16 b92; s16 b94; s16 b96; s32 b98; s32 b9c; } taiiku_tama_gfrm_tick = {0};
struct { s16 x; s16 y; } taiiku_tama_yf = {0, 0};
struct { s32 x; s32 y; } taiiku_tama_ppush = {0, 0};
static u8 pad_align[12] = {0};
struct { s32 b58; s32 b5c; u16 b60; u16 b62; s32 b64; s32 b68; } taiiku_tama_push = {0};

extern s32 taiiku_tama_fall;
extern s32 taiiku_tama_pfrm;
extern s32 taiiku_tama_gfrm;
extern s16 taiiku_tama_gcue;
extern s32 taiiku_tama_xf;
extern s32 taiiku_tama_zone;
extern s32 taiiku_tama_girl;
extern s32 taiiku_course_no;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s16 cw97_x;
extern s16 cw97_y;
extern s16 cal_cw_frame;
extern s16 cw97_frame;
extern u8  cal_cw_disp;
extern u8  cw97_disp;
extern volatile s16 cw101_x;
extern CLASS_W class_w[];
extern s16 taiiku_tama_ang[];

void taiiku_state_reset(void)
{
    s32 n;

    taiiku_tama_fall = 0;
    taiiku_tama_pfrm = 0;
    taiiku_tama_ppush.x = 0;
    taiiku_tama_ppush.y = 0;
    taiiku_tama_gfrm = 0;
    taiiku_tama_gfrm_tick.b92 = 0;
    taiiku_tama_gfrm_tick.b98 = 0;
    taiiku_tama_gfrm_tick.b9c = 0;
    taiiku_tama_gcue = 0;
    taiiku_tama_xf = 0;
    taiiku_tama_yf.x = 0;
    taiiku_tama_yf.y = 128;
    taiiku_tama_push.b58 = 0;
    taiiku_tama_push.b5c = 0;
    taiiku_tama_push.b64 = 0;
    taiiku_tama_push.b68 = 0;
    taiiku_tama_ang[1] = 0;
    taiiku_tama_zone = 1;
    class_w[96].x.half[0] = 0;
    cal_cw_x = 32;
    class_w[96].y.half[0] = 0;
    cal_cw_y = 48;
    class_w[97].x.half[0] = 0;
    n = taiiku_course_no;
    if (n % 3 == 0) {
        goto neg32;
    }
    if (taiiku_tama_girl != 2) {
        goto zeroit;
    }
neg32:
    cw97_x = -32;
    goto after;
zeroit:
    cw97_x = 0;
after:
    class_w[97].y.half[0] = 0;
    cw97_y = 48;
    cal_cw_frame = 0;
    cw97_frame = taiiku_tama_girl * 6 + 6;
    cal_cw_disp = 164;
    cw97_disp = 164;
    cw101_x = 160;
}
