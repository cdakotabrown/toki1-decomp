/* @PostProc: swap_instructions --offset1 0x144 --offset2 0x154 */
/* @PostProc: swap_instructions --offset1 0x148 --offset2 0x154 */
/* @PostProc: swap_instructions --offset1 0x14c --offset2 0x154 */
/* Address: 0x80149508 */
#include "types.h"

typedef struct {
    s32 a;
    s32 b;
    s32 c;
    u8 d;
    u8 e;
    u8 f;
    u8 g;
} Cfg80146;

extern Cfg80146 pslt[];
extern s32 taco_obj_1_attribute[][4];
extern volatile s32 step_counter;

extern void goto_sub_step();
extern void image_upload_queue_add_flags();
extern void GsSetFlatLight();
extern void sd_cmd_limit_send();
extern void taco_tile_load();
extern void taco_rec_move();
extern void taco_band_disp();

void taco_mainc_main3(void)
{
    s32 n;
    s32 i;
    u32 m;

    taco_rec_move(0, 3, 5, -20, 0, 0, 0, 0, 0);
    n = step_counter;
    if (n >= 180 && n < 199) {
        taco_band_disp(1, 224, 224, 224);
    } else if (n == 199) {
        sd_cmd_limit_send(0, 1284);
    } else if (n >= 200 && n < 320) {
        if (n % 3 == 0) {
            taco_band_disp(1, 240, 240, 240);
        }
    } else if (n >= 320 && n < 440) {
        m = (u32)(n * 2 - 640) & 0xFF;
        taco_band_disp(2, m, m & 0xFF, (u8)m);
    } else if (n == 440) {
        for (i = 0; i < 35; i++) {
            taco_obj_1_attribute[i][0] = (s32)0x80000000;
        }
        image_upload_queue_add_flags(0x80162000, 12, 1, 0, 0, 256, 256);
        taco_tile_load(0x80172000, 14);

        pslt[0].a = 20;
        pslt[0].b = 100;
        pslt[0].c = -100;
        pslt[0].d = 176;
        pslt[0].e = 176;
        pslt[0].f = 176;
        GsSetFlatLight(0, &pslt[0]);

        pslt[1].a = 20;
        pslt[1].b = -100;
        pslt[1].c = 100;
        pslt[1].d = 128;
        pslt[1].e = 128;
        pslt[1].f = 128;
        GsSetFlatLight(1, &pslt[1]);

        pslt[2].a = -20;
        pslt[2].b = 20;
        pslt[2].c = -100;
        pslt[2].d = 96;
        pslt[2].e = 96;
        pslt[2].f = 96;
        GsSetFlatLight(2, &pslt[2]);

        goto_sub_step(31);
    }
    step_counter = step_counter + 1;
}
