/* Address: 0x8013A030 */
/* Size: 96 bytes (24 instructions) */
#include "types.h"

extern u8 D_800B1AF5;
extern u8 D_800B1AF6;
extern s32 D_8011F720;

extern void func_80078950(s32 *a0, u8 a1, u8 a2);
extern void func_800FE090(void);
extern void func_80015FE0(void);

void func_8013A030(void) {
    func_80078950(&D_8011F720, D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_800FE090();
    } else {
        func_80015FE0();
    }
}
