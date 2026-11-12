/* Address: 0x8013C144 */
/* Size: 60 bytes (15 instructions) */
#include "types.h"

extern s32 D_80094764;
extern s32 D_8011F784;
extern s16 D_800EAFB6;

extern void func_80078970();
extern void func_80011DFC();

void func_8013C144(void) {
    s32 pad[4];
    (void)&pad;
    func_80078970(&D_80094764, &D_8011F784);
    D_800EAFB6 = 4;
    func_80011DFC();
}
