/* @CompileFlags: -Wc,-pic0 */
/* master_bukatu_bustup_read - MASTER overlay, Address: 0x80139810, Size: 256 bytes (64 insns) */
/* @SymbolSize: 256 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v1:0 */
#include "types.h"

typedef struct {
    s32 w[13];
} Table52;

extern Table52 master_bukatu_bustup_read_tbl;
extern volatile u8 now_sub_sub_step;
extern u8 master_bukatu_girl_no;

extern void bustup_cd_read(s32);
extern void sub_step_up(void);
extern void sub_sub_step_up(void);
extern void fade_out_step(void);
extern void goto_sub_step(s32);
extern void game_reset(void);
extern void common_disp(void);

void master_bukatu_bustup_read(void) {
    s32 padB;
    u8 state;
    Table52 buf;
    s32 padA[3];

    state = now_sub_sub_step;
    buf = master_bukatu_bustup_read_tbl;

    switch (state) {
    case 0:
        if (master_bukatu_girl_no != 255) {
            master_bukatu_girl_no = master_bukatu_girl_no;
            bustup_cd_read(buf.w[master_bukatu_girl_no]);
            sub_step_up();
        } else {
            master_bukatu_girl_no = 255;
            sub_sub_step_up();
        }
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        goto_sub_step(3);
        break;
    default:
        game_reset();
        break;
    }

    common_disp();
}
