/* @SymbolStart: 0x80139B30 */
/* @SymbolSize: 76 */
/* Address: 0x80139B54 (true start 0x80139B30) */
/* Size: 76 bytes (19 instructions, head + body) */
#include "types.h"

extern s32 D_80120620;
extern s32 D_80120640;
extern s32 D_80120660;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;

extern void func_80011DFC(void);

void func_80139B54(void)
{
    D_80120678 = D_80120620;
    D_8012067C = D_80120640;
    D_80120680 = D_80120660;
    func_80011DFC();
}
