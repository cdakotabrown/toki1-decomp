/* Address: 0x80143354 */
/* Size: 204 bytes (51 instructions) -- TARGET: 51, COMPILED: 44 */
#include "types.h"

extern s32 D_80123430;
extern s32 D_80123434;
extern s32 D_80123438;
extern s16 D_8012343C;
extern s32 D_80123440;
extern s32 D_80123444;
extern s32 D_80123448;
extern s32 D_8012344C;
extern s32 D_80123450;

extern s32 D_80123454;
extern s32 D_80123458;

extern void func_80106C20(void);
extern void func_80012D64();
extern void func_8004C46C();
extern void func_8004C6B0();
extern void func_80011DFC();

void func_80143354(void) {
    func_80106C20();
    func_80012D64(D_80123440, 17, 1, 2, 0);
    func_8004C46C(D_80123444, D_80123448, D_8012344C, D_80123450, D_80123454, D_80123458);
    func_8004C6B0(D_80123434, D_80123438, D_80123430, D_8012343C);
    func_80011DFC();
}
