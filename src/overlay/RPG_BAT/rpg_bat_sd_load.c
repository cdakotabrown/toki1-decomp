/* @SymbolStart: 0x8013D834 */
/* @SymbolSize: 1068 */
/* @RodataBase: 0x8015BCB0 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=1 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x24:rt:v1:v0 0x28:rs:v1:v0 0x28:rt:v1:v0 0x30:rs:v1:v0 0x3c:rs:v1:v0 0x48:rs:v1:v0 0x54:rs:v1:v0 0x60:rs:v1:v0 0x6c:rs:v1:v0 0x78:rs:v0:v1 0x9c:rt:v0:v1 0xa0:rs:v0:v1 0xa0:rt:v0:v1 0xa8:rs:v0:v1 0xcc:rt:v0:v1 0xd0:rs:v0:v1 0xd0:rt:v0:v1 0xd8:rs:v0:v1 0xfc:rt:v0:v1 0x100:rs:v0:v1 0x100:rt:v0:v1 0x108:rs:v0:v1 0x12c:rt:v0:v1 0x130:rs:v0:v1 0x130:rt:v0:v1 0x138:rs:v0:v1 0x15c:rt:v0:v1 0x160:rs:v0:v1 0x160:rt:v0:v1 0x168:rs:v0:v1 0x1bc:rt:v0:v1 0x1c0:rs:v0:v1 0x1c0:rt:v0:v1 0x1c8:rs:v0:v1 0x1f0:rt:v0:v1 0x1f4:rs:v0:v1 0x1f4:rt:v0:v1 0x1fc:rs:v0:v1 0x224:rt:v0:v1 0x228:rs:v0:v1 0x228:rt:v0:v1 0x230:rs:v0:v1 0x244:rs:v0:v1 0x25c:rt:t4:v1 0x260:rs:t4:v1 0x260:rt:t4:v1 0x268:rs:t4:v1 0x268:rt:t5:t4 0x270:rt:t5:t4 0x278:rs:v0:v1 0x290:rt:t6:v1 0x294:rs:t6:v1 0x294:rt:t6:v1 0x29c:rs:t6:v1 0x29c:rt:t7:t5 0x2a4:rt:t7:t5 0x2ac:rs:v0:v1 0x2b0:rt:t8:t6 0x2b4:rs:t8:t6 0x2b4:rt:t8:t6 0x2bc:rs:t8:t6 0x2c0:rs:v0:v1 0x2c4:rt:t9:t7 0x2c8:rs:t9:t7 0x2c8:rt:t9:t7 0x2d0:rs:t9:t7 0x2d8:rd:t9:t7 0x2d8:rt:t9:t7 0x2e0:rt:t9:t7 0x2e4:rt:t9:t7 0x2ec:rs:t9:t7 0x310:rt:v0:v1 0x314:rs:v0:v1 0x314:rt:v0:v1 0x31c:rs:v0:v1 0x31c:rt:t0:t8 0x33c:rt:v0:v1 0x340:rs:v0:v1 0x340:rt:v0:v1 0x348:rs:v0:v1 0x348:rt:t0:t8 0x368:rt:v0:v1 0x36c:rs:v0:v1 0x36c:rt:v0:v1 0x374:rs:v0:v1 0x374:rt:t0:t8 0x394:rt:v0:v1 0x398:rs:v0:v1 0x398:rt:v0:v1 0x3a0:rs:v0:v1 0x3a0:rt:t0:t8 0x3ac:rt:t0:t8 0x3c0:rs:v0:v1 0x3d8:rt:t2:v1 0x3dc:rs:t2:v1 0x3dc:rt:t2:v1 0x3e4:rs:t2:v1 0x3e4:rt:t3:t2 0x3ec:rt:t3:t2 0x3f4:rs:v0:v1 0x40c:rt:t4:v1 0x410:rs:t4:v1 0x410:rt:t4:v1 0x418:rs:t4:v1 0x418:rt:t5:t3 0x420:rt:t5:t3 */
/* rpg_bat_sd_load - RPG_BAT overlay, Address: 0x8013D83C, Size: 1060 bytes */
/* 2. `@CompileEnv: IDO52_CONST_NOINREG_VALUES=1` (Hook, const-web remat). */
#include "types.h"

extern s32 rpg_bat_mode;
extern s32 rpg_bat_girl;
extern s32 rpg_bat_bukatu_no;
extern s32 rpg_bat_bukatu_flag;
extern u8 step_mode;

extern void device_load_start();
extern s32 sub_step_up();
extern s32 device_load_poll();
extern s32 device_ready_check();

void rpg_bat_sd_load(void)
{
    s32 step;
    s32 sel;

    step = step_mode;
    if (step == 0) {
        sel = rpg_bat_mode;
        switch (sel) {
        case 0x00010000:
            device_load_start(1, 0xC3B6, 0xC39B, 0xD209, 0xD1BB, 0xD1B0);
            break;
        case 0x00020000:
            device_load_start(1, 0xC399, 0xC37E, 0xD209, 0xD1BB, 0xD1B0);
            break;
        case 0x00040000:
            device_load_start(1, 0xC37C, 0xC363, 0xD209, 0xD1BB, 0xD1B0);
            break;
        case (s32)0x80000000:
            device_load_start(1, 0xC43E, 0xC410, 0xD209, 0xD1BB, 0xD1B0);
            break;
        case 0x08000000:
            device_load_start(1, 0xC3D7, 0xC3B8, 0xD209, 0xD1BB, 0xD1B0);
            break;
        case 0x00100000:
            switch ((u32)rpg_bat_girl) {
            case 2:
            case 4:
            case 5:
                device_load_start(1, 0xC2F6, 0xC2C4, 0xD209, 0xD1BB, 0xD1B0);
                break;
            case 0:
            case 1:
            case 3:
            case 10:
                device_load_start(1, 0xC32D, 0xC2F8, 0xD209, 0xD1BB, 0xD1B0);
                break;
            case 6:
            case 7:
            case 8:
            case 9:
                device_load_start(1, 0xC361, 0xC32F, 0xD209, 0xD1BB, 0xD1B0);
                break;
            }
            break;
        }
        step_mode = step_mode + 1;
    } else if (step == 1) {
        if (device_load_poll() == 1) {
            step_mode = step_mode + 1;
        }
    } else if (step == 2) {
        if (device_ready_check(1) == 1) {
            step_mode = step_mode + 1;
        }
    } else if (step == 3) {
        if (rpg_bat_bukatu_flag == 1) {
            switch ((u32)rpg_bat_bukatu_no) {
            case 1:
            case 5:
            case 6:
            case 7:
            case 8:
            case 10:
                device_load_start(0, 0xC23C, 0xC21F, 0, 0, 0);
                break;
            case 9:
            case 11:
                device_load_start(0, 0xC252, 0xC23E, 0, 0, 0);
                break;
            case 0:
            case 3:
            case 4:
                device_load_start(0, 0xC273, 0xC254, 0, 0, 0);
                break;
            case 2:
                device_load_start(0, 0xC294, 0xC275, 0, 0, 0);
                break;
            }
            step_mode = step_mode + 1;
        } else {
            step_mode = step_mode + 3;
        }
    } else if (step == 4) {
        if (device_load_poll() == 1) {
            step_mode = step_mode + 1;
        }
    } else if (step == 5) {
        if (device_ready_check(0) == 1) {
            step_mode = step_mode + 1;
        }
    } else {
        sub_step_up();
    }
}
