/* Address: 0x8013A270 */
/* Size: 168 bytes (42 instructions) */
#include "types.h"

extern s32 D_80121800;
extern s32 D_80121804;
extern s32 D_80121808;
extern s16 D_8012180C;
extern s32 D_80121810;
extern s32 D_80121814;
extern s32 D_80121818;
extern s32 D_8012181C;
extern s32 D_80121820;
extern s32 D_80121824;
extern s32 D_80121828;

extern void func_800FDC20(void);
extern void func_80012D64();
extern void func_8004C46C();
extern void func_8004C6B0();
extern void func_80011DFC();

void func_8013A270(void) {
    func_800FDC20();
    func_80012D64(D_80121810, 17, 1, 2, 0);
    func_8004C46C(D_80121814, D_80121818, D_8012181C, D_80121820, D_80121824, D_80121828);
    func_8004C6B0(D_80121804, D_80121808, D_80121800, D_8012180C);
    func_80011DFC();
}
