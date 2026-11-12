/* @SymbolSize: 1108 */
/* @PostProc: scheduler_normalizer --const-fp --store-fp --branch-fp */
/* Address: 0x8014E5C8 */
/* Size: 1108 bytes (277 insns) */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern s32 pslt;
extern u8 *taco_actor_rec;

extern void taco_rec_move(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g,
                          s32 h, s32 i);
extern void taco_angle_tables_init(void);
extern void taco_line_seq(s32 a);
extern void GsSetFlatLight(s32 idx, void *p);

void taco_act5_run(s32 sel)
{
    u8 *p;

    if (sel < 19) {
        taco_rec_move(0, 0, 0, -400, 0, 0, 0, 50, 50);
    }
    if (sel == 19) {
        taco_actor_rec[3126] = 1;
        taco_angle_tables_init();
    }
    if (sel >= 20 && sel < 39) {
        if (taco_actor_rec[3126] != 0) {
            taco_line_seq(0);
        }
    }
    if (sel == 39) {
        pslt = 0;
        ((LIBGS_F_LIGHT *)&pslt)[0].vy = 0;
        ((LIBGS_F_LIGHT *)&pslt)[0].vz = -100;
        ((LIBGS_F_LIGHT *)&pslt)[0].r = 255;
        ((LIBGS_F_LIGHT *)&pslt)[0].g = 255;
        ((LIBGS_F_LIGHT *)&pslt)[0].b = 255;
        GsSetFlatLight(0, &pslt);
        ((LIBGS_F_LIGHT *)&pslt)[1].vx = 0;
        ((LIBGS_F_LIGHT *)&pslt)[1].vy = 0;
        ((LIBGS_F_LIGHT *)&pslt)[1].vz = -100;
        ((LIBGS_F_LIGHT *)&pslt)[1].r = 255;
        ((LIBGS_F_LIGHT *)&pslt)[1].g = 255;
        ((LIBGS_F_LIGHT *)&pslt)[1].b = 255;
        GsSetFlatLight(1, &((LIBGS_F_LIGHT *)&pslt)[1].vx);
        ((LIBGS_F_LIGHT *)&pslt)[2].vx = 0;
        ((LIBGS_F_LIGHT *)&pslt)[2].vy = 0;
        ((LIBGS_F_LIGHT *)&pslt)[2].vz = -100;
        ((LIBGS_F_LIGHT *)&pslt)[2].r = 255;
        ((LIBGS_F_LIGHT *)&pslt)[2].g = 255;
        ((LIBGS_F_LIGHT *)&pslt)[2].b = 255;
        GsSetFlatLight(2, &((LIBGS_F_LIGHT *)&pslt)[2].vx);
    }
    if (sel >= 40 && sel < 67) {
        taco_rec_move(0, 0, 0, 800, 0, 0, 0, 20, 30);
    }
    if (sel >= 70 && sel < 100) {
        taco_rec_move(0, 0, 0, -480, 0, 0, 0, 50, 50);
    }
    if (sel == 100) {
        taco_actor_rec[3262] = 1;
        taco_actor_rec[2994] = 0;
        taco_actor_rec[2995] = taco_actor_rec[2994];
        pslt = -100;
        ((LIBGS_F_LIGHT *)&pslt)[0].vy = 30;
        ((LIBGS_F_LIGHT *)&pslt)[0].vz = -30;
        ((LIBGS_F_LIGHT *)&pslt)[0].r = 224;
        ((LIBGS_F_LIGHT *)&pslt)[0].g = 224;
        ((LIBGS_F_LIGHT *)&pslt)[0].b = 224;
        GsSetFlatLight(0, &pslt);
        ((LIBGS_F_LIGHT *)&pslt)[1].vx = 40;
        ((LIBGS_F_LIGHT *)&pslt)[1].vy = 50;
        ((LIBGS_F_LIGHT *)&pslt)[1].vz = -100;
        ((LIBGS_F_LIGHT *)&pslt)[1].r = 64;
        ((LIBGS_F_LIGHT *)&pslt)[1].g = 64;
        ((LIBGS_F_LIGHT *)&pslt)[1].b = 64;
        GsSetFlatLight(1, &((LIBGS_F_LIGHT *)&pslt)[1].vx);
        ((LIBGS_F_LIGHT *)&pslt)[2].vx = 10;
        ((LIBGS_F_LIGHT *)&pslt)[2].vy = -20;
        ((LIBGS_F_LIGHT *)&pslt)[2].vz = -100;
        ((LIBGS_F_LIGHT *)&pslt)[2].r = 48;
        ((LIBGS_F_LIGHT *)&pslt)[2].g = 48;
        ((LIBGS_F_LIGHT *)&pslt)[2].b = 48;
        GsSetFlatLight(2, &((LIBGS_F_LIGHT *)&pslt)[2].vx);
    }
    if (sel >= 101 && sel < 121) {
        if (taco_actor_rec[3126] != 0) {
            taco_line_seq(0);
        }
    }
    if (sel >= 66 && sel < 72) {
        p = taco_actor_rec;
        p[2177] = 64;
        p = taco_actor_rec;
        *(s16 *)(p + 2296) = 2048;
        p = taco_actor_rec;
        p[2308] = 10;
        p = taco_actor_rec;
        p[2313] = 64;
        p = taco_actor_rec;
        *(s16 *)(p + 2432) = 2048;
        p = taco_actor_rec;
        p[2444] = 30;
        p = taco_actor_rec;
        p[2449] = 64;
        p = taco_actor_rec;
        *(s16 *)(p + 2568) = 2048;
        p = taco_actor_rec;
        p[2580] = 30;
    } else if (sel == 72) {
        p = taco_actor_rec;
        p[2177] = 128;
        p = taco_actor_rec;
        p[2313] = 128;
        p = taco_actor_rec;
        p[2449] = 128;
    }
}
