/* Address: 0x80146490 */
/* Size: 96 bytes (24 instructions) */
#include "types.h"

extern u8 D_800B1AF5;
extern u8 D_800B1AF6;
extern s32 D_8011FAE8;

extern void func_80078950(s32 *a0, u8 a1, u8 a2);
extern void func_8010AE7C(void);
extern void func_80015FE0(void);

void func_80146490(void) {
    func_80078950(&D_8011FAE8, D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_8010AE7C();
    } else {
        func_80015FE0();
    }
}
