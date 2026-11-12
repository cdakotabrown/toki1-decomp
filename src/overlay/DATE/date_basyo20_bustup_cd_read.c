/* @SymbolStart: 0x8013BF94 */
/* @SymbolSize: 172 */
/* @RodataBase: 0x8015AF84 */
/* Function: date_basyo20_bustup_cd_read */
/* Overlay: DATE */
/* Address: 0x8013BF94 (cross-prologue head) / 0x8013BF9C (body) */
/* Size: 172 bytes (43 instructions) */
#include "types.h"

extern u8 now_girl;

extern void bustup_cd_read(s32);

void date_basyo20_bustup_cd_read(void) {
    switch (now_girl) {
    case 0: bustup_cd_read(22163); break;
    case 4: bustup_cd_read(26618); break;
    case 5: bustup_cd_read(24818); break;
    case 6: bustup_cd_read(25448); break;
    case 7: bustup_cd_read(23603); break;
    case 8: bustup_cd_read(20858); break;
    default: bustup_cd_read(27248); break;
    }
}
