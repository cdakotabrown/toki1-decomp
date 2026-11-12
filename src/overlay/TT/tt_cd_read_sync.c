/* @SymbolStart: 0x80142DDC */
/* @SymbolSize: 608 */
/* tt_cd_read_sync - TT overlay, Address: 0x80142DF0, Size: 588 bytes */
/* `@CompileFlags: -Wc,-pic0` stamp compensating for the invented padding */
#include "types.h"

extern s32 CdSync();
extern s32 CdControl();
extern s32 CdReadSync();
extern s32 CdRead();

void tt_cd_read_sync(s32 arg0, s32 arg1, s32 arg2)
{
    u8 buf[4];
    s32 f;
    s32 s;
    s32 m;

    f = arg1 % 75;
    buf[2] = (u8)(f / 10 * 16 + f % 10);
    s = arg1 / 75;
    m = s / 60;
    buf[0] = (u8)(m / 10 * 16 + m % 10);
    m = s % 60;
    buf[1] = (u8)(m / 10 * 16 + m % 10);

    while (CdControl(2, buf, 0) == 0) {
    }
    while (CdSync(1, 0) != 2) {
    }
    CdRead((arg2 + 2047) / 2048, arg0, 128);
    while (CdReadSync(1, 0) > 0) {
    }
}
