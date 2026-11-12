/* @SymbolStart: 0x8013DC60 */
/* @SymbolSize: 1692 */
/* @RodataBase: 0x8015BD0C */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=1 IDO52_UGEN_NUM_TREGS=3 */
/* @PostProc: patch_insn_reg_field 0x160:rt:t9:v1 0x164:rt:t8:v1 0x170:rt:t8:v1 0x174:rs:t9:v1 0x174:rt:t9:v1 0x17c:rs:t9:v1 0x17c:rt:t0:t8 0x184:rt:t0:t8 0x1a4:rt:t1:v1 0x1a8:rs:t1:v1 0x1a8:rt:t1:v1 0x1b0:rs:t1:v1 0x1b0:rt:t2:t9 0x1b8:rt:t2:t9 0x1d0:rt:t6:t0 0x1d4:rs:t6:t0 0x1d8:rt:t7:v1 0x1dc:rs:t7:v1 0x1dc:rt:t7:v1 0x1e4:rs:t7:v1 0x1e4:rt:t8:t1 0x1ec:rt:t8:t1 0x1f8:rt:t9:t2 0x1fc:rs:t9:t2 0x1fc:rt:t9:t2 0x204:rs:t9:t2 0x208:rt:t2:t6 0x20c:rt:t0:t3 0x210:rs:t0:t3 0x210:rt:t0:t3 0x218:rs:t0:t3 0x220:rd:t0:t3 0x220:rt:t0:t3 0x228:rt:t0:t3 0x22c:rt:t0:t3 0x234:rs:t0:t3 0x25c:rt:t1:t4 0x280:rt:t1:t4 0x2a4:rt:t1:t4 0x2c8:rt:t1:t4 0x2ec:rt:t1:t4 0x310:rt:t1:t4 0x334:rt:t1:t4 0x358:rt:t1:t4 0x37c:rt:t1:t4 0x3a0:rt:t1:t4 0x3c4:rt:t1:t4 0x3e8:rt:t1:t4 0x3f4:rt:t1:t4 0x400:rt:t2:t6 0x418:rt:t6:t7 0x41c:rs:t6:t7 0x420:rt:t7:v1 0x424:rs:t7:v1 0x424:rt:t7:v1 0x42c:rs:t7:v1 */
/* rpg_bat_cd_read - RPG_BAT overlay, Address: 0x8013DC68 */
/* Size: 1684 bytes (421 insns) + the 8-byte cross-prologue head at 0x8013DC60, */
#include "types.h"

extern s32 sub_step_up();
extern s32 get_CD_State2();
extern s32 cd_read_start2();

extern u8 step_mode;
extern s32 rpg_bat_mode;
extern s32 rpg_bat_girl;
extern s32 rpg_bat_bukatu_no;
extern s32 rpg_bat_bukatu_flag;

s32 rpg_bat_cd_read(void)
{
    if (step_mode == 0) {
        switch (rpg_bat_mode) {
        case 0x00010000:
            cd_read_start2(128, (void *)0x80180000, 14934);
            break;
        case 0x00020000:
            cd_read_start2(112, (void *)0x80180000, 15637);
            break;
        case 0x00040000:
            cd_read_start2(112, (void *)0x80180000, 15273);
            break;
        case 0x80000000:
            cd_read_start2(153, (void *)0x80180000, 15859);
            break;
        case 0x08000000:
            cd_read_start2(149, (void *)0x80180000, 15454);
            break;
        case 0x00100000:
            cd_read_start2(98, (void *)0x80180000, 14699);
            break;
        }
        step_mode = step_mode + 1;
        return;
    }
    if (step_mode == 1) {
        if (get_CD_State2() & 1) {
            if (rpg_bat_mode != 0x00100000) {
                step_mode = 4;
            } else {
                step_mode = step_mode + 1;
            }
        }
        return;
    }
    if (step_mode == 2) {
        cd_read_start2(43, (void *)0x801B1000, 15411);
        step_mode = step_mode + 1;
        return;
    }
    if (step_mode == 3) {
        if (get_CD_State2() & 1) {
            step_mode = step_mode + 1;
        }
        return;
    }
    if (step_mode == 4) {
        if (rpg_bat_bukatu_flag == 1) {
            switch (rpg_bat_bukatu_no) {
            case 0:
                cd_read_start2(33, (void *)0x801D8000, 14856);
                break;
            case 1:
                cd_read_start2(33, (void *)0x801D8000, 14604);
                break;
            case 2:
                cd_read_start2(34, (void *)0x801D8000, 15603);
                break;
            case 3:
                cd_read_start2(45, (void *)0x801D8000, 14889);
                break;
            case 4:
                cd_read_start2(45, (void *)0x801D8000, 14637);
                break;
            case 5:
                cd_read_start2(34, (void *)0x801D8000, 14822);
                break;
            case 6:
                cd_read_start2(25, (void *)0x801D8000, 16055);
                break;
            case 7:
                cd_read_start2(26, (void *)0x801D8000, 15749);
                break;
            case 8:
                cd_read_start2(41, (void *)0x801D8000, 15818);
                break;
            case 9:
                cd_read_start2(43, (void *)0x801D8000, 15775);
                break;
            case 10:
                cd_read_start2(25, (void *)0x801D8000, 14797);
                break;
            case 11:
                cd_read_start2(59, (void *)0x801D8000, 15096);
                break;
            }
            step_mode = step_mode + 1;
        } else {
            step_mode = step_mode + 2;
        }
        return;
    }
    if (step_mode == 5) {
        if (get_CD_State2() & 1) {
            step_mode = step_mode + 1;
        }
        return;
    }
    if (step_mode == 6) {
        switch (rpg_bat_girl) {
        case 0:
            cd_read_start2(9, (void *)0x801C7000, 15155);
            break;
        case 1:
            cd_read_start2(17, (void *)0x801C7000, 15214);
            break;
        case 2:
            if (rpg_bat_mode != 0x80000000) {
                cd_read_start2(34, (void *)0x801C7000, 15062);
            } else {
                sub_step_up();
            }
            break;
        case 3:
            cd_read_start2(25, (void *)0x801C7000, 15189);
            break;
        case 4:
            cd_read_start2(17, (void *)0x801C7000, 15394);
            break;
        case 5:
            cd_read_start2(17, (void *)0x801C7000, 15256);
            break;
        case 6:
            cd_read_start2(25, (void *)0x801C7000, 15231);
            break;
        case 7:
            cd_read_start2(25, (void *)0x801C7000, 15164);
            break;
        case 8:
            cd_read_start2(17, (void *)0x801C7000, 14682);
            break;
        case 9:
            cd_read_start2(9, (void *)0x801C7000, 15385);
            break;
        case 10:
            cd_read_start2(17, (void *)0x801C7000, 16105);
            break;
        case 13:
            cd_read_start2(25, (void *)0x801C7000, 16080);
            break;
        case 14:
            sub_step_up();
            break;
        }
        step_mode = step_mode + 1;
        return;
    }
    if (step_mode == 7) {
        if (get_CD_State2() & 1) {
            sub_step_up();
        }
        return;
    }
    sub_step_up();
}
