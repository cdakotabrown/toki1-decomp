/* Address: 0x801444A0 */
/* Size: 96 bytes (24 instructions) */
#include "types.h"

extern u8 D_800B1AF5;
extern u8 D_800B1AF6;
extern s32 D_8011FA30;

extern void func_80078950(s32 *a0, u8 a1, u8 a2);
extern void func_801085C0(void);
extern void func_80015FE0(void);

void func_801444A0(void) {
    func_80078950(&D_8011FA30, D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_801085C0();
    } else {
        func_80015FE0();
    }
}
