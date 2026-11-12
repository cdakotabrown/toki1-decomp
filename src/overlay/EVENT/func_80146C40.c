/* @CompileEnv: IDO52_CONST_INREG_VALUES=8 */
/* Address: 0x80146C40 */
/* Size: 184 bytes (46 instructions) */
#include "types.h"

extern u8 D_800EAFA4;
extern u8 D_800EAFE8;
extern s32 D_80123EAC;
extern s32 D_80123EB0;
extern s32 D_80123EB4;
extern s16 D_80123EB8;
extern s32 D_80123EBC;
extern s32 D_80123EC0;
extern s32 D_80123EC4;
extern s32 D_80123EC8;
extern s32 D_80123ECC;
extern s32 D_80123ED0;
extern s32 D_80123ED4;

extern void func_8010A280(void);
extern void func_80012D64();
extern void func_8004C46C();
extern void func_8004C6B0();
extern void func_80011DFC(void);

void func_80146C40(void) {
    func_8010A280();
    func_80012D64(D_80123EBC, 17, 1, 2, 0);
    func_8004C46C(D_80123EC0, D_80123EC4, D_80123EC8, D_80123ECC, D_80123ED0, D_80123ED4);
    func_8004C6B0(D_80123EB0, D_80123EB4, D_80123EAC, D_80123EB8);
    D_800EAFA4 = 8;
    D_800EAFE8 = 8;
    func_80011DFC();
}
