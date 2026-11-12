/* @RodataBase: 0x8015D66C */
/* @SymbolStart: 0x80134D9C */
/* @SymbolSize: 304 */
/* (was @SymbolSize: 276) */
/* Address: 0x80134DB8 */
#include "types.h"

extern s32 step_state;

extern void sd_cmd_limit_send(s32, s32);
extern void taco_light(void);
extern void taco_init0(void);
extern void rand_tile_disp(void);
extern void taco_pad_wait_step(void);
extern void taco_slot_wait2(void);
extern void white_light_down(void);
extern void taco_pass4(void);
extern void taco_reset5(void);

typedef struct {
    u32 frame;
    u8  pad4;
    u8  step1;
    u8  step;
    u8  pad7;
} TacoStep;
extern TacoStep step_counter;

void taco_slot_dispatch(void)
{
    step_counter.frame += 1;
    switch (step_counter.step) {
        case 0: taco_init0(); break;
        case 1: taco_pad_wait_step(); break;
        case 2: taco_slot_wait2(); break;
        case 3: white_light_down(); break;
        case 4: taco_pass4(); break;
        case 5: taco_reset5(); break;
    }
    taco_light();
    rand_tile_disp();
    if (step_state == 160) {
        sd_cmd_limit_send(0, 1284);
    }
    if (step_state == 64) {
        sd_cmd_limit_send(0, 1285);
    }
    if (step_state == 128) {
        sd_cmd_limit_send(0, 1286);
    }
    if (step_state == 192) {
        sd_cmd_limit_send(0, 1287);
    }
}
