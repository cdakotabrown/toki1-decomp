/* Address: 0x801463D0 */
/* Size: 96 bytes (24 instructions) */
#include "types.h"

extern void func_80078950(void *arg0, u8 arg1, u8 arg2);
extern void func_8010A4F0(void);
extern void func_80015FE0(void);

extern u8 D_800B1AF5;
extern u8 D_800B1AF6;
extern u8 D_8011FAD0[];

void func_801463D0(void) {
    u8 a1 = D_800B1AF5;
    u8 a2 = D_800B1AF6;

    func_80078950(D_8011FAD0, a1, a2);

    if (D_800B1AF5 == 0) {
        func_8010A4F0();
    } else {
        func_80015FE0();
    }
}
