/* Address: 0x80146AD0 (true start 0x80146AAC: the first copy's lui/lw are */
/* Size: 0x80146AF8 - 0x80146AAC = 76 bytes (19 instructions) */
/* @SymbolStart: 0x80146AAC */
/* @SymbolSize: 76 */
#include "types.h"

extern s32 D_8012062C;
extern s32 D_8012064C;
extern s32 D_8012066C;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;

extern void func_80011DFC(void);

void func_80146AD0(void) {
    D_80120678 = D_8012062C;
    D_8012067C = D_8012064C;
    D_80120680 = D_8012066C;
    func_80011DFC();
}
