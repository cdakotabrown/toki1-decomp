/* Address: 0x800593FC */
/* Size: 80 bytes */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void GsSetDrawBuffOffset(void);
extern void GsDefDispBuff(s32 a0, s32 a1, s32 a2, s32 a3);
extern void GsInit3D(void);
extern void InitGeom(void);
extern s32 GsGetActiveBuff(void);
extern s32 activeBuff;

void gs_env_init(void) {
    GsSetDrawBuffOffset();
    GsDefDispBuff(0, 0, 0, 240);
    GsInit3D();
    InitGeom();
    activeBuff = GsGetActiveBuff();
}
