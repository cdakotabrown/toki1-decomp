/* Size: 196 bytes */
/* @SymbolSize: 196 */
#include "types.h"
typedef struct { u8 b[3267]; } Blob;
extern Blob D_80146428;
extern u8 D_800F62CF;
extern u8 D_800F53DE;
extern s32 D_8012E66C;
extern u8 D_800D9334[];
extern void func_800BCE10(u8 *, u8 *);
void func_80135E84(void)
{
    s32 pad;
    Blob buf;
    buf = D_80146428;
    func_800BCE10(D_800D9334, &buf.b[D_800F62CF * 297 + D_800F53DE * 99 + D_8012E66C * 33 - 9504]);
}
