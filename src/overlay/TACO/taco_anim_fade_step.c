/* @SymbolStart: 0x8014F5C0 */
/* @SymbolSize: 560 */
/* @PostProc: swap_instructions --offset1 0xdc --offset2 0xe4 */
/* @PostProc: patch_insn_reg_field 0xac:rt:t8:t9 0xb8:rt:t9:t0 0xbc:rt:v0:a1 0xc0:rs:v0:a1 0xc0:rt:v0:a1 0xc8:rs:v0:a1 0xfc:rt:t8:t9 0x104:rt:t9:t0 0x10c:rt:t0:t1 0x110:rt:t0:t1 0x118:rt:t1:t2 0x11c:rt:t1:t2 0x124:rt:t2:t3 0x128:rt:t2:t3 0x130:rt:t3:t4 0x138:rt:t3:t4 0x154:rt:t4:t5 0x158:rt:t4:t5 0x160:rt:t5:t6 0x164:rt:t5:t6 0x16c:rt:t6:t7 0x170:rt:t6:t7 0x178:rt:t7:t8 0x17c:rt:t7:t8 0x184:rt:t8:t9 0x18c:rt:t8:t9 0x19c:rt:t9:t0 0x1a4:rt:t9:t0 0x1ac:rt:t0:t1 0x1b0:rt:t0:t1 0x1b8:rt:t1:t2 0x1bc:rt:t1:t2 0x1c4:rt:t2:t3 0x1c8:rt:t2:t3 0x1d0:rt:t3:t4 0x1d4:rt:t3:t4 0x1dc:rt:t4:t5 0x1e4:rt:t4:t5 0x204:rt:v0:v1 0x20c:rs:v0:v1 0x220:rt:v0:v1 0x22c:rs:v0:v1 */
/* Address: 0x8014F5C8 */
/* Size: 552 bytes (138 insns) */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern u8  back_clear_r;
extern u8  back_clear_g;
extern u8  back_clear_b;
extern s32 step_counter;

extern s32 pslt;

extern void sub_sub_step_up(void);
extern void GsSetFlatLight(s32 idx, void *p);
extern void sd_cmd_limit_send(s32 a, s32 b);
extern void taco_anim_disp(s32 a);
extern void taco_anim(s32 a);
extern void taco_fade_disp(s32 a);
extern void taco_list_chan_rand_set(s32 a, s32 b, s32 c, s32 d);

void taco_anim_fade_step(void)
{
    s32 n;
    s32 v;

    if ((u32)step_counter < 100U) {
        taco_anim_disp(1);
        taco_anim(5);
        taco_list_chan_rand_set(0x801A7000, 10, 256, 495);
    } else if (((u32)step_counter >= 100U) && ((u32)step_counter < 500U)) {
        taco_anim(5);
        taco_anim_disp((s32)((u32)(step_counter - 60) / 40U));
    } else if ((u32)step_counter >= 550U) {
        if ((u32)back_clear_r < 255U) {
            n = back_clear_b + 1;
            back_clear_g = n;
            back_clear_r = n;
            step_counter = step_counter;
            back_clear_b = n;
            taco_fade_disp(step_counter - 550);
        } else {
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
            step_counter = 0;
            sub_sub_step_up();
        }
    }
    v = step_counter;
    if (v == 600) {
        sd_cmd_limit_send(0, 1282);
        v = step_counter;
    }
    step_counter = v + 1;
}
