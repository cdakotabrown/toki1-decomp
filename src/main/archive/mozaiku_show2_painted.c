/* Function: mozaiku_show2 */
/* Address: 0x800563DC */
/* Size: 532 bytes (133 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 24 --new 40 */
/* @PostProc: fill_delay_slots */
/* @PostProc: scheduler_normalizer */
/* @PostProc: custom_script custom_mozaiku_show2.py */
/* @CompileFlags: -Wo,-loopunroll,0 */
#include "types.h"

extern void strcpy(u8 *, void *);
extern void strcat(u8 *, void *);

extern u8 D_800B9C24[];
extern s32 D_800B6570[];
extern s32 D_800B6574[];
extern s32 D_800B6578[];
extern s32 D_800B657C[];
extern s32 D_800B6580[];
extern s32 D_800B6584[];
extern s32 D_800B6588[];
extern s32 D_800B658C[];
extern s32 D_800B6590[];
extern s32 D_800B6594[];
extern s32 D_800B6598[];
extern s32 D_800B659C[];
extern s32 D_800B65A0[];
extern s32 D_800B65A4[];
extern s32 D_800B65A8[];
extern s32 D_800B65AC[];

s32 mozaiku_show2(u8 *arg0, s32 arg1) {
    s32 i;
    u8 *ptr;

    ptr = arg0;
    for (i = 0; i < 20; i += 4) {
        ptr[0] = 0;
        ptr[1] = 0;
        ptr[2] = 0;
        ptr[3] = 0;
        ptr += 4;
    }

    strcpy(arg0, D_800B9C24);

    switch (arg1) {
        case 0:
            strcat(arg0, D_800B6570);
            return 1;
        case 1:
            strcat(arg0, D_800B6574);
            return 1;
        case 2:
            strcat(arg0, D_800B6578);
            return 1;
        case 3:
            strcat(arg0, D_800B657C);
            return 1;
        case 4:
            strcat(arg0, D_800B6580);
            return 1;
        case 5:
            strcat(arg0, D_800B6584);
            return 1;
        case 6:
            strcat(arg0, D_800B6588);
            return 1;
        case 7:
            strcat(arg0, D_800B658C);
            return 1;
        case 8:
            strcat(arg0, D_800B6590);
            return 1;
        case 9:
            strcat(arg0, D_800B6594);
            return 1;
        case 10:
            strcat(arg0, D_800B6598);
            return 1;
        case 11:
            strcat(arg0, D_800B659C);
            return 1;
        case 12:
            strcat(arg0, D_800B65A0);
            return 1;
        case 13:
            strcat(arg0, D_800B65A4);
            return 1;
        case 14:
            strcat(arg0, D_800B65A8);
            return 1;
        case 15:
            strcat(arg0, D_800B65AC);
            return 1;
        default:
            return 0;
    }
}
