/* @RodataBase: 0x80148DEC */
#include "types.h"

extern void taiiku_scroll0(void);
extern void func_8013B5F8(void);
extern void taiiku_tama_girl2_event_step(void);
extern void taiiku_tama_girl3_event_step(void);
extern void taiiku_tama_girl4_event_step(void);
extern void taiiku_tama_girl5_event_step(void);
extern void taiiku_tama_girl6_event_step(void);
extern void taiiku_tama_girl7_event_step(void);
extern void taiiku_scroll8(void);
extern void taiiku_scroll9(void);
extern void taiiku_tama_girl10_event_step(void);
extern void game_reset(void);
extern s32 taiiku_tama_girl;

void taiiku_event_main(void) {
    s32 cmd = taiiku_tama_girl;

    switch (cmd) {
    case 0:
        taiiku_scroll0();
        break;
    case 1:
        func_8013B5F8();
        break;
    case 2:
        taiiku_tama_girl2_event_step();
        break;
    case 3:
        taiiku_tama_girl3_event_step();
        break;
    case 4:
        taiiku_tama_girl4_event_step();
        break;
    case 5:
        taiiku_tama_girl5_event_step();
        break;
    case 6:
        taiiku_tama_girl6_event_step();
        break;
    case 7:
        taiiku_tama_girl7_event_step();
        break;
    case 8:
        taiiku_scroll8();
        break;
    case 9:
        taiiku_scroll9();
        break;
    case 10:
        taiiku_tama_girl10_event_step();
        break;
    default:
        game_reset();
        break;
    }
}
