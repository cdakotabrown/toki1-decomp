/* Address: 0x801450D4 */
/* Size: 168 bytes (42 instructions) */
#include "types.h"

extern s32 D_80123880;
extern s32 D_80123884;
extern s32 D_80123888;
extern s16 D_8012388C;
extern s32 D_80123890;
extern s32 D_80123894;
extern s32 D_80123898;
extern s32 D_8012389C;
extern s32 D_801238A0;

extern s32 D_801238A4;
extern s32 D_801238A8;

extern void func_80107FC0(void);
extern void func_80012D64();
extern void func_8004C46C();
extern void func_8004C6B0();
extern void func_80011DFC();

void func_801450D4(void) {
    func_80107FC0();
    func_80012D64(D_80123890, 17, 1, 2, 0);
    func_8004C46C(D_80123894, D_80123898, D_8012389C, D_801238A0, D_801238A4, D_801238A8);
    func_8004C6B0(D_80123884, D_80123888, D_80123880, D_8012388C);
    func_80011DFC();
}
