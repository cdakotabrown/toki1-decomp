/* Function: GsDefDispBuff */
/* Address: 0x80099480 (the recorded symbol 0x800994A4 is a HEAD -- see */
/* Size: 152 bytes (38 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O1 --konami-lib-sched */
/* @SymbolStart: 0x80099480 */
/* @SymbolSize: 152 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: maspsx --aspsx-version=2.67 */
#include "types.h"

extern s16 PSDGPU;
extern s16 PSDBASEX;
extern s16 libgs_dispbuff_x1;
extern s16 PSDBASEY;
extern s16 libgs_dispbuff_y1;
extern s16 PSDOFSX;
extern s16 libgs_psdofsx_buf1;
extern s16 PSDOFSY;
extern s16 libgs_psdofsy_buf1;
extern void GsSetDrawBuffClip(void);
extern void GsSetDrawBuffOffset(void);

void GsDefDispBuff(s16 p0, s16 p1, s16 p2, s16 p3)
{
    s32 mute;

    mute = PSDGPU;
    PSDOFSX = p0;
    libgs_psdofsx_buf1 = p2;
    PSDOFSY = p1;
    libgs_psdofsy_buf1 = p3;
    if (mute != 0) {
        PSDBASEX = 0;
        libgs_dispbuff_x1 = 0;
        PSDBASEY = 0;
        libgs_dispbuff_y1 = 0;
    } else {
        PSDBASEX = p0;
        libgs_dispbuff_x1 = p2;
        PSDBASEY = p1;
        libgs_dispbuff_y1 = p3;
    }
    GsSetDrawBuffClip();
    GsSetDrawBuffOffset();
}
