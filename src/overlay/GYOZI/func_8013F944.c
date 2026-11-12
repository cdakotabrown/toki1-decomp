/* func_8013F944 -- GYOZI overlay, Address: 0x8013F944, Size: 336 bytes */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v1:8 */
#include "types.h"

typedef s32 (*FuncPtr)(s32);

typedef struct {
    FuncPtr t[26];
} FuncTable26;

typedef struct {
    u8 pad0;
    u8 c7;
    s16 c8;
    u8 pad1[12];
    s16 d6;
    s16 d8;
} GyoziState;

extern FuncTable26 D_80148880;
extern GyoziState D_8012B8C6;
extern u8 D_800F647A;
extern u8 D_801317EB;
extern u8 D_80148820;
extern u8 D_80148824;

extern void func_801428EC(void);
extern void func_80142C14(void);

s32 func_8013F944(void)
{
    s32 sel;
    FuncTable26 localTable;

    localTable = D_80148880;
    localTable.t[D_800F647A](128);

    sel = D_800F647A;
    if ((u32)sel < 8) {
        D_8012B8C6.c7 = D_801317EB;
        if (D_8012B8C6.d8 == 0 && D_8012B8C6.c8 == 0) {
            D_8012B8C6.d6 = (s16)(D_8012B8C6.d6 + 1);
            if (D_8012B8C6.d6 >= 3) {
                D_8012B8C6.d6 = 0;
            }
        }
    }
    if ((u32)sel >= 10) {
        if (D_80148820 != 0) {
            func_801428EC();
            sel = D_800F647A;
        }
    }
    if ((u32)sel >= 10) {
        if (D_80148824 != 0) {
            func_80142C14();
        }
    }
}
