/* func_8013ECE8 -- EVENT overlay, Address: 0x8013ECE8 */
/* @SymbolStart: 0x8013ECC4 */
/* @SymbolSize: 124 */
#include "types.h"

extern s32 D_80120448;
extern s32 D_80120464;
extern s32 D_80120480;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_80094784;
extern s32 D_8011F8B0;
extern volatile s16 D_800B0B0A;

extern void func_80078970();
extern void func_8004C250();
extern void func_80011DFC();

void func_8013ECE8(void) {
    D_80120678 = D_80120448;
    D_8012067C = D_80120464;
    D_80120680 = D_80120480;
    func_80078970(&D_80094784, &D_8011F8B0);
    D_800B0B0A = (s16)(D_800B0B0A + 1);
    func_8004C250();
    func_80011DFC();
}
