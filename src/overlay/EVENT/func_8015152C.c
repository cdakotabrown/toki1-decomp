/* Address: 0x8015152C */
/* Size: 168 bytes (42 instructions) */
#include "types.h"

extern s32 ev_flag18;
extern s32 D_80094784;
extern s32 D_8011FEC4;
extern u8 D_800B1746;
extern u8 D_800B0A2B;
extern s16 D_80094730;
extern u8 D_8012531C;

extern void func_800433D0();
extern void func_80078970();
extern void func_800469F4();
extern void func_80011DFC();

void func_8015152C(void) {
    s32 v0;

    func_800433D0(512);
    func_80078970(&D_80094784, &D_8011FEC4);
    func_800469F4(17124);
    if ((&D_800B0A2B)[D_800B1746 * 52] & 1) {
        goto set1;
    }
    v0 = 0;
    goto done;
set1:
    v0 = 1;
done:
    D_80094730 = D_80094730 + v0 * 2;
    ev_flag18 = 19;
    D_8012531C = v0;
    func_80011DFC();
}
