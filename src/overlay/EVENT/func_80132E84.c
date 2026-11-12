/* @RodataBase: 0x8011F540 */
/* Address: 0x80132E84 */
/* Size: 376 bytes (94 instructions) */
#include "types.h"

extern void func_8004ACA8(void);
extern void func_800F9CFC(void);
extern void func_80101510(void);
extern void func_800FBF40(void);
extern void func_800FDF90(void);
extern void func_80106F90(void);
extern void func_801027E0(void);
extern void func_80103FF0(void);
extern void func_800FFF50(void);
extern void func_800F7690(void);
extern void func_80105460(void);
extern void func_801083E0(void);
extern void func_8010A330(void);
extern void func_800FD360(void);
extern void func_8010B410(void);
extern void func_80015FE0(void);
extern void func_80046A7C(void);
extern void func_8004BB54(void);
extern void func_800372E0(s32 arg);
extern void func_800358D0(void);
extern void func_80036A8C(void);
extern void func_80035774(void);
extern void func_8004AEB0(void);
extern void func_80017020(void);

extern s32 D_800EECB4;

void func_80132E84(void)
{
    func_8004ACA8();

    switch ((u32)D_800EECB4) {
    case 0:
        func_800F9CFC();
        break;
    case 1:
        func_80101510();
        break;
    case 2:
        func_800FBF40();
        break;
    case 3:
        func_800FDF90();
        break;
    case 4:
        func_80106F90();
        break;
    case 5:
        func_801027E0();
        break;
    case 6:
        func_80103FF0();
        break;
    case 7:
        func_800FFF50();
        break;
    case 8:
        func_800F7690();
        break;
    case 9:
        func_80105460();
        break;
    case 10:
        func_801083E0();
        break;
    case 11:
        func_8010A330();
        break;
    case 12:
        func_800FD360();
        break;
    case 13:
        func_8010B410();
        break;
    default:
        func_80015FE0();
        break;
    }

    func_80046A7C();
    func_8004BB54();
    func_800372E0(2);
    func_800358D0();
    func_80036A8C();
    func_80035774();
    func_8004AEB0();
    func_80017020();
}
