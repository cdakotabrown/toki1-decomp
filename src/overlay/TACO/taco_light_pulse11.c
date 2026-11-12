/* @SymbolSize: 628 */
/* @PostProc: patch_insn_reg_field 0x1c4:rt:t0:t1 0x1c8:rt:t0:t1 0x1d0:rt:t1:t0 0x1d4:rt:t1:t0 0x258:rt:t0:t1 0x25c:rs:t0:t1 0x25c:rt:t0:t1 0x264:rs:t0:t1 0x26c:rt:t1:t0 0x270:rt:t1:t0 */
/* Address: 0x80148D3C */
/* Size: 628 bytes (157 insns) */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern s32 pslt;
extern u8 taco_main2_hp_tbl;
extern u8 *taco_actor_rec;

extern s32 get_taco_percent(s32 a, void *p);
extern void GsSetFlatLight(s32 idx, void *p);

void taco_light_pulse11(void)
{
    s32 ret;
    u8 *ptr;
    s32 v;

    ret = get_taco_percent(11, &taco_main2_hp_tbl);
    ptr = taco_actor_rec;
    if ((u32)ptr[2722] < (u32)ret) {
        pslt = 0;
        ((LIBGS_F_LIGHT *)&pslt)[0].vy = 0;
        ((LIBGS_F_LIGHT *)&pslt)[0].vz = -100;
        ((LIBGS_F_LIGHT *)&pslt)[0].r = 255;
        ((LIBGS_F_LIGHT *)&pslt)[0].g = 128;
        ((LIBGS_F_LIGHT *)&pslt)[0].b = 128;
        GsSetFlatLight(0, &pslt);
        ((LIBGS_F_LIGHT *)&pslt)[1].vx = 0;
        ((LIBGS_F_LIGHT *)&pslt)[1].vy = 0;
        ((LIBGS_F_LIGHT *)&pslt)[1].vz = -100;
        ((LIBGS_F_LIGHT *)&pslt)[1].r = 255;
        ((LIBGS_F_LIGHT *)&pslt)[1].g = 128;
        ((LIBGS_F_LIGHT *)&pslt)[1].b = 128;
        GsSetFlatLight(1, &((LIBGS_F_LIGHT *)&pslt)[1].vx);
        ((LIBGS_F_LIGHT *)&pslt)[2].vx = 0;
        ((LIBGS_F_LIGHT *)&pslt)[2].vy = 0;
        ((LIBGS_F_LIGHT *)&pslt)[2].vz = -100;
        ((LIBGS_F_LIGHT *)&pslt)[2].r = 255;
        ((LIBGS_F_LIGHT *)&pslt)[2].g = 128;
        ((LIBGS_F_LIGHT *)&pslt)[2].b = 128;
        GsSetFlatLight(2, &((LIBGS_F_LIGHT *)&pslt)[2].vx);
        ptr = taco_actor_rec;
        v = ptr[2723];
        if ((u32)v >= 3) {
            ptr[2722] = ret;
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
            ptr = taco_actor_rec;
            ptr[2723] = 0;
        } else {
            ptr = taco_actor_rec;
            ptr[2723] = v + 1;
        }
    }
}
