/* Address: 0x80133740 */
/* Size: 96 bytes (24 instructions) */
#include "types.h"

extern u8 D_800B1AF5;
extern u8 D_800B1AF6;
extern u8 D_8011F580;

extern void func_80078950(u8 *a0, u8 a1, u8 a2);
extern void func_800F77A0(void);
extern void func_80015FE0(void);

void func_80133740(void) {
    func_80078950(&D_8011F580, D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_800F77A0();
    } else {
        func_80015FE0();
    }
}
