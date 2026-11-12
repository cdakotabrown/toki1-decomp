/* @RodataBase: 0x8015D5AC */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:16,v0:7 */
/* @SymbolStart: 0x80133E98 */
/* @PostProc: swap_instructions --offset1 0xf0 --offset2 0xf4 */
/* @PostProc: swap_instructions --offset1 0x3bc --offset2 0x3c0 */
/* @PostProc: swap_instructions --offset1 0x3c0 --offset2 0x3c4 */
/* @PostProc: patch_insn_reg_field 0x10:rt:t6:v0 0x14:rs:t6:v0 0x14:rt:t6:v0 0x1c:rs:t6:v0 0x28:rt:t6:v0 0x84:rt:v0:t7 0x8c:rs:v0:t7 0x90:rt:t7:t8 0x94:rs:t7:t8 0x9c:rt:t7:t8 0xa8:rt:t8:t9 0xac:rs:t8:t9 0xc4:rt:t9:t0 0xc8:rs:t9:t0 0xd0:rt:t9:t0 0xe8:rt:t0:v0 0xec:rs:t0:v0 0xec:rt:t0:v0 0xf8:rs:t0:v0 0x3b4:rt:t4:v0 0x3b8:rs:t4:v0 0x3b8:rt:t4:v0 0x3c8:rs:t4:v0 0x3c8:rt:t5:t4 0x3cc:rt:t5:t4 0x3ec:rt:t6:t5 0x3f0:rs:t6:t5 0x408:rt:t7:t6 0x40c:rs:t7:t6 0x414:rt:t7:t6 0x418:rt:t8:t7 0x41c:rs:t8:t7 0x41c:rt:t8:t7 0x424:rt:t8:t7 0x42c:rt:t9:t8 0x430:rt:t9:t8 0x438:rt:t0:t9 0x440:rt:t0:t9 */
/* @SymbolSize: 1092 */
/* Address: 0x80133E98 (absorbed cross-prologue head) / 0x80133EA0 (prologue) */
/* Size: 1092 bytes (273 insns) */
#include "types.h"

extern void cd_read_start(s32, s32, s32);
extern s32 get_CD_State2(void);
extern s32 printf(u8 *, s32);
extern s32 sub_sub_step_up(void);

extern u8 step_mode;
extern u8 taco_zk05_adr_fmt[];
extern u8 *taco_actor_rec;
extern u8 cw2_active[];
extern s32 class_max;
extern s32 object3d_top;

s32 taco_zk_cd_load(void) {
    u32 w;

    switch (step_mode) {
    case 0:
        cd_read_start(1, 0x8018A000, 0xB3F2);
        cd_read_start(5, 0x8018A800, 0xB413);
        cd_read_start(18, 0x8018D000, 0xB438);
        w = (u8)(step_mode + 2);
        step_mode = w;
        return w;
    case 1:
    case 2:
    case 3:
    case 4:
        if (!(get_CD_State2() & 1)) {
            return;
        }
        w = (u8)(step_mode + 1);
        step_mode = w;
        return w;
    case 5:
        cd_read_start(7, 0x80196000, 0xB45B);
        step_mode = step_mode + 1;
        cd_read_start(3, 0x80199800, 0xB478);
        cd_read_start(2, 0x8019B000, 0xB47B);
        w = (u8)(step_mode + 1);
        step_mode = w;
        return w;
    case 6:
    case 7:
    case 8:
    case 9:
        if (!(get_CD_State2() & 1)) {
            return;
        }
        w = (u8)(step_mode + 1);
        step_mode = w;
        return w;
    case 10:
        cd_read_start(1, 0x8019C000, 0xB486);
        cd_read_start(2, 0x8019C800, 0xB487);
        cd_read_start(4, 0x8019D800, 0xB4A9);
        cd_read_start(6, 0x8019F800, 0xB4AD);
        w = (u8)(step_mode + 1);
        step_mode = w;
        return w;
    case 11:
    case 12:
    case 13:
    case 14:
        if (!(get_CD_State2() & 1)) {
            return;
        }
        w = (u8)(step_mode + 1);
        step_mode = w;
        return w;
    case 15:
        cd_read_start(2, 0x801A2800, 0xB4B3);
        cd_read_start(4, 0x801A3800, 0xB4B5);
        cd_read_start(3, 0x801A5800, 0xB4B9);
        cd_read_start(10, 0x801A7000, 0xB515);
        w = (u8)(step_mode + 1);
        step_mode = w;
        return w;
    case 16:
    case 17:
    case 18:
    case 19:
        if (!(get_CD_State2() & 1)) {
            return;
        }
        w = (u8)(step_mode + 1);
        step_mode = w;
        return w;
    case 20:
        cd_read_start(1, 0x801AC000, 0xB534);
        cd_read_start(3, 0x801AC800, 0xB668);
        cd_read_start(3, 0x801AE000, 0xB66B);
        cd_read_start(3, 0x801AF800, 0xB66E);
        w = (u8)(step_mode + 1);
        step_mode = w;
        return w;
    case 21:
    case 22:
    case 23:
    case 24:
        if (!(get_CD_State2() & 1)) {
            return;
        }
        w = (u8)(step_mode + 1);
        step_mode = w;
        return w;
    case 25:
        cd_read_start(2, 0x801B1000, 0xB671);
        cd_read_start(3, 0x801B2000, 0xB673);
        cd_read_start(3, 0x801B3800, 0xB676);
        cd_read_start(2, 0x801B5000, 0xB679);
        step_mode = step_mode + 1;
        return printf(taco_zk05_adr_fmt, 0x801B5000);
    case 26:
    case 27:
    case 28:
    case 29:
        if (!(get_CD_State2() & 1)) {
            return;
        }
        w = (u8)(step_mode + 1);
        step_mode = w;
        return w;
    case 30:
        taco_actor_rec = cw2_active;
        class_max = 1;
        object3d_top = 2;
        return sub_sub_step_up();
    }
}
