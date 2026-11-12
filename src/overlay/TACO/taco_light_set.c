/* @SymbolSize: 624 */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=2 */
/* @PostProc: patch_insn_reg_field 0x1c8:rt:t0:t1 0x1cc:rt:t0:t1 0x1d4:rt:t1:t0 0x1d8:rt:t1:t0 0x260:rt:t1:t0 0x264:rs:t1:t0 0x264:rt:t1:t0 0x268:rt:t0:t1 0x26c:rs:t1:t0 0x26c:rt:t0:t1 */
/* Address: 0x8014A8B0 */
/* Size: 624 bytes (156 insns) */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern s32 pslt;
extern u8 taco_main16_hp_tbl;
extern u8 *taco_actor_rec;

extern s32 get_taco_percent(s32 a, void *p);
extern void GsSetFlatLight(s32 idx, void *p);

void taco_light_set(void)
{
    u32 ret;
    u8 *ptr;
    s32 v;

    ret = get_taco_percent(11, &taco_main16_hp_tbl);
    ptr = taco_actor_rec;
    if (ptr[2858] < ret) {
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
        v = ptr[2859];
        if ((u32)v >= 3) {
            ptr[2858] = ret;
            pslt = 100;
            ((LIBGS_F_LIGHT *)&pslt)[0].vy = 50;
            ((LIBGS_F_LIGHT *)&pslt)[0].vz = -100;
            ((LIBGS_F_LIGHT *)&pslt)[0].r = 112;
            ((LIBGS_F_LIGHT *)&pslt)[0].g = 112;
            ((LIBGS_F_LIGHT *)&pslt)[0].b = 128;
            GsSetFlatLight(0, &pslt);
            ((LIBGS_F_LIGHT *)&pslt)[1].vx = 0;
            ((LIBGS_F_LIGHT *)&pslt)[1].vy = -100;
            ((LIBGS_F_LIGHT *)&pslt)[1].vz = -100;
            ((LIBGS_F_LIGHT *)&pslt)[1].r = 64;
            ((LIBGS_F_LIGHT *)&pslt)[1].g = 64;
            ((LIBGS_F_LIGHT *)&pslt)[1].b = 64;
            GsSetFlatLight(1, &((LIBGS_F_LIGHT *)&pslt)[1].vx);
            ((LIBGS_F_LIGHT *)&pslt)[2].vx = -100;
            ((LIBGS_F_LIGHT *)&pslt)[2].vy = 50;
            ((LIBGS_F_LIGHT *)&pslt)[2].vz = -100;
            ((LIBGS_F_LIGHT *)&pslt)[2].r = 128;
            ((LIBGS_F_LIGHT *)&pslt)[2].g = 128;
            ((LIBGS_F_LIGHT *)&pslt)[2].b = 144;
            GsSetFlatLight(2, &((LIBGS_F_LIGHT *)&pslt)[2].vx);
            ptr = taco_actor_rec;
            ptr[2859] = 0;
        } else {
            ptr = taco_actor_rec;
            ptr[2859] = v + 1;
        }
    }
}
