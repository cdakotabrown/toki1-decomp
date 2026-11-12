/* @CompileFlags: -cckr -Wc,-pic0 */
/* @CompileEnv: IDO52_LDA_NOINREG_CLASSES=1:1:2,1:4:4,1:6:6 IDO52_NO_LOOP_TEST_REPLACEMENT=bb:8,11 IDO52_FORCE_WEB_COLOR=5:v1 */
/* @SymbolSize: 848 */
/* Address: 0x80138B84 */
/* Size: 848 bytes (212 insns) */
#include "types.h"

typedef struct { s32 w[2]; }  Blk8;
typedef struct { s32 w[18]; } Blk72;
typedef struct { s32 w[5]; }  Blk20;
typedef struct { s32 w[4]; }  Blk16;

extern Blk8  taiiku_load2_main_tbl0;
extern Blk72 taiiku_load2_main_tbl1;
extern Blk20 taiiku_load2_main_tbl2;

extern u8  now_sub_sub_step;
extern s32 screen_w[];
extern s32 bukatu_tbl[];
extern s32 tpage_buf[];
extern s32 taiiku_tama_girl_tpg[];
extern s32 taiiku_tama_field_tpg[];
extern s32 taiiku_tama_plate_tpg[];
extern s32 taiiku_tama_buf_1;

extern sub_step_up();
extern sub_sub_step_up();
extern palette_upload_enqueue();
extern get_CD_State2();
extern cd_read_start2();
extern game_reset();
extern search_load_tpage_buf_lock();
extern class_clear_all();
extern set_flag_b3_high_bit();
extern date_x_taku_set();
extern Fade_Out_Init();
extern bri_fade_out();
extern taiiku_race_init2();
extern taiiku_vram_init();
extern taiiku_girl_sprite_init();
extern taiiku_tbl_init();
extern taiiku_bufptr_init2();

taiiku_load2_main()
{
    s32 pa, pb, pc;
    Blk8  b164;
    Blk72 b92;
    Blk20 b72;
    s32 *so;
    s32 *sq;
    s32 *dstp;
    s32 *dstq;
    s32  i;
    s32  r;

    b164 = taiiku_load2_main_tbl0;
    b92 = taiiku_load2_main_tbl1;
    b72 = taiiku_load2_main_tbl2;

    switch (now_sub_sub_step) {
    case 0:
        date_x_taku_set(1, 1);
        if (0) {}
        for (i = 0; i < 3; i++) {
            screen_w[i + 1167] = 8;
        }
        for (r = 0; r < 3; r++) {
            screen_w[r + 1199] = 8;
        }
        tpage_buf[28] = 8;
        class_clear_all(0);
        set_flag_b3_high_bit(0);
        if (0) {}
        dstp = taiiku_tama_girl_tpg;
        for (so = b164.w; so < &b164.w[2]; so++) {
            *dstp++ = *so;
        }
        dstq = taiiku_tama_field_tpg;
        for (sq = b92.w; sq < &b92.w[18]; sq++) {
            *dstq++ = *sq;
        }
        for (i = 0; i < 5; i++) {
            taiiku_tama_plate_tpg[i] = b72.w[i];
        }
        sub_sub_step_up();
        break;
    case 1:
        cd_read_start2(30, 0x80180000, 0xBAD7);
        Fade_Out_Init();
        sub_sub_step_up();
        break;
    case 2:
        bri_fade_out();
        if (get_CD_State2() & 1) {
            cd_read_start2(46, 0x8018F000, 0xBAF5);
            sub_sub_step_up();
        }
        break;
    case 3:
        if (bri_fade_out() != 0) {
            if (get_CD_State2() & 1) {
                taiiku_tbl_init();
                taiiku_bufptr_init2();
                dstp = taiiku_tama_girl_tpg;
                for (i = 0; i < 2; i++) {
                    r = search_load_tpage_buf_lock(*dstp, 1);
                    tpage_buf[r] = i + 0x80000300;
                    dstp++;
                    dstp[-1] = i + 0x80000300;
                }
                for (i = 0; i != 16; i++) {
                    palette_upload_enqueue(taiiku_tama_buf_1, (i + 16) & 0xFF, 1, 1, i);
                }
                cd_read_start2(185, 0x8018F000, 0xBA1E);
                taiiku_race_init2();
                taiiku_vram_init();
                taiiku_girl_sprite_init();
                sub_step_up();
            }
        }
        break;
    default:
        game_reset();
        break;
    }
}
