/* Address: 0x8014B340 */
/* Size: 376 bytes (94 insns) */
/* @CompileEnv: IDO52_COPYPROP_VAR=-4 IDO52_COPYPROP_SITE=lod:2:deny */
#include "types.h"

extern u8 D_80080714;
extern s16 D_80094730;
extern u8 D_800B086A;
extern u8 D_800B0F2C;
extern u8 D_800B0F30;
extern s32 D_800B18D0;
extern u8 D_800B1888;
extern u8 D_800B1746;
extern u32 D_800B1AE4;
typedef struct {
    u8 pad0[7];
    u8 voice;
    u8 pad1[48];
    u32 bits;
    u8 pad2[8];
} EvRec;

extern EvRec D_800E9E18[];
extern u32 D_800EC190[];
extern s32 D_800EECB4;
extern u8 D_800F19AB;
extern s16 D_80124384;
extern s16 D_80124388;
extern u8 D_801243B0;
extern u8 D_801243B4;
extern u8 D_801252E4;

extern void func_80012D2C(s32, s32);
extern void func_8001886C(s32);
extern void func_80010678(void);
extern void func_80018944(s32);
extern void func_80012D40(s32);
extern void func_8004CDDC(void);
extern void func_8001FD50(void);
extern void func_800201FC(s32);
extern void func_8003535C(void);
extern void func_8003580C(void);
extern void func_8004BC20(s32);
extern void func_8004C360(void);
extern void func_80011DFC(void);

void func_8014B340(void) {
    s32 c;
    u32 flags;
    u8 b;

    func_80012D2C(1, 0);
    func_8001886C(1);
    func_80010678();
    func_80018944(0);
    func_80012D40(1);
    func_8004CDDC();
    D_800B086A = 128;
    D_800F19AB = 0;
    D_80080714 = 0;
    D_800B1888 = 0;
    D_800B18D0 = 1;
    func_8001FD50();
    func_800201FC(1);
    func_8003535C();
    func_8003580C();
    b = D_800B0F2C;
    c = D_800B0F30;
    D_801243B0 = b;
    D_800EECB4 = c;
    flags = D_800B1AE4;
    D_80094730 = 0;
    D_801252E4 = 1;
    if ((flags & 3) == 0 && c != 2) {
        D_801252E4 = 0;
    }
    D_801243B4 = 0;
    D_80124384 = 0;
    D_80124388 = 0;
    D_800B1746 = c;
    func_8004BC20(c);
    D_800E9E18[1].voice = 0;
    D_800E9E18[1].bits = D_800E9E18[1].bits | 0x81000000;
    D_800E9E18[0].bits = D_800E9E18[0].bits | 0x81000000;
    D_800EC190[0] = D_800EC190[0] | 0x81000000;
    D_800EC190[9] = D_800EC190[9] | 0x81000000;
    func_8004C360();
    func_80011DFC();
}
