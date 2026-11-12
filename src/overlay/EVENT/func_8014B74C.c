/* @SymbolSize: 1728 */
/* @RodataBase: 0x8011FBD0 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-4 */
/* Function: func_8014B74C */
/* Overlay: EVENT */
/* Address: 0x8014B74C */
/* Size: 1728 bytes (432 instructions; one alignment nop follows) */
#include "types.h"

typedef void (*StepFn)(s32);

typedef struct { StepFn f[18]; } StepTbl18;
typedef struct { StepFn f[29]; } StepTbl29;
typedef struct { StepFn f[31]; } StepTbl31;
typedef struct { StepFn f[32]; } StepTbl32;
typedef struct { StepFn f[34]; } StepTbl34;
typedef struct { StepFn f[35]; } StepTbl35;
typedef struct { StepFn f[36]; } StepTbl36;
typedef struct { StepFn f[37]; } StepTbl37;
typedef struct { StepFn f[39]; } StepTbl39;
typedef struct { StepFn f[41]; } StepTbl41;
typedef struct { StepFn f[45]; } StepTbl45;
typedef struct { StepFn f[47]; } StepTbl47;
typedef struct { StepFn f[48]; } StepTbl48;
typedef struct { StepFn f[61]; } StepTbl61;
typedef struct { StepFn f[103]; } StepTbl103;

extern u8 D_801243B0;
extern u8 D_800B1AF6;
extern s32 ev_flag24;

extern StepTbl18 D_801244AC;
extern StepTbl35 D_801244F4;
extern StepTbl48 D_80124580;
extern StepTbl32 D_80124640;
extern StepTbl35 D_801246C0;
extern StepTbl34 D_8012474C;
extern StepTbl36 D_801247D4;
extern StepTbl37 D_80124864;
extern StepTbl36 D_801248F8;
extern StepTbl39 D_80124988;
extern StepTbl32 D_80124A24;
extern StepTbl32 D_80124AA4;
extern StepTbl103 D_80124B24;
extern StepTbl29 D_80124CC0;
extern StepTbl45 D_80124D34;
extern StepTbl41 D_80124DE8;
extern StepTbl37 D_80124E8C;
extern StepTbl47 D_80124F20;
extern StepTbl61 D_80124FDC;
extern StepTbl31 D_801250D0;
extern StepTbl31 D_8012514C;

extern void func_8011B848(void);
extern void func_8011C164(void);
extern void func_8011C11C(void);

void func_8014B74C(void)
{
    s32 step;
    StepTbl18 t0;
    StepTbl35 t1;
    StepTbl48 t2;
    StepTbl32 t3;
    StepTbl35 t4;
    StepTbl34 t5;
    StepTbl36 t6;
    StepTbl37 t7;
    StepTbl36 t8;
    StepTbl39 t9;
    StepTbl32 t10;
    StepTbl32 t11;
    StepTbl103 t12;
    StepTbl29 t13;
    StepTbl45 t14;
    StepTbl41 t15;
    StepTbl37 t16;
    StepTbl47 t17;
    StepTbl61 t18;
    StepTbl31 t19;
    StepTbl31 t20;
    StepFn *p;

    t0 = D_801244AC;
    t1 = D_801244F4;
    t2 = D_80124580;
    t3 = D_80124640;
    t4 = D_801246C0;
    t5 = D_8012474C;
    t6 = D_801247D4;
    t7 = D_80124864;
    t8 = D_801248F8;
    t9 = D_80124988;
    t10 = D_80124A24;
    t11 = D_80124AA4;
    t12 = D_80124B24;
    t13 = D_80124CC0;
    t14 = D_80124D34;
    t15 = D_80124DE8;
    t16 = D_80124E8C;
    t17 = D_80124F20;
    t18 = D_80124FDC;
    t19 = D_801250D0;
    t20 = D_8012514C;

    switch (D_801243B0) {
    case 0:
        p = t0.f;
        break;
    case 1:
        p = t1.f;
        break;
    case 2:
        p = t2.f;
        break;
    case 3:
        p = t3.f;
        break;
    case 4:
        p = t4.f;
        break;
    case 5:
        p = t5.f;
        break;
    case 6:
        p = t6.f;
        break;
    case 7:
        p = t7.f;
        break;
    case 8:
        p = t8.f;
        break;
    case 9:
        p = t9.f;
        break;
    case 10:
        p = t10.f;
        break;
    case 11:
        p = t11.f;
        break;
    case 12:
        p = t12.f;
        break;
    case 13:
        p = t13.f;
        break;
    case 14:
        p = t14.f;
        break;
    case 15:
        p = t15.f;
        break;
    case 16:
        p = t16.f;
        break;
    case 17:
        p = t17.f;
        break;
    case 18:
        p = t18.f;
        break;
    case 19:
        p = t19.f;
        break;
    case 20:
        p = t20.f;
        break;
    default:
        p = t0.f;
        break;
    }
    p[D_800B1AF6](128);
    if (ev_flag24 != 0) {
        func_8011B848();
    }
    if (p == t1.f && D_800B1AF6 >= 12U && D_800B1AF6 < 21U) {
        func_8011C164();
    }
    step = D_800B1AF6;
    if (p == t1.f && step == 21) {
        func_8011C11C();
    }
}
