/* Function: ContNrpn2 */
/* Address: 0x80093890  Size: 264 bytes (66 instructions) */
/* Region: Library (0x80086000+)   Compiler: IDO 5.2 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_LA_PULL=all IDO52_AS1_NO_FALLTHRU_FILL=all */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
/* @SymbolStart: 0x80093890 */
/* @SymbolSize: 264 */
#include "types.h"

extern u32 _ss_score[];
extern s32 ReadDeltaValue(s16, s16);

void ContNrpn2(s16 a0, s16 a1, u32 a2) {
    u8 *ptr;
    u8 temp;

    a2 = a2 & 0xFF;
    ptr = (u8 *)_ss_score[a0] + a1 * 168;

    switch (a2) {
    case 20:
        ptr[22] = (u8)a2;
        ptr[39] = 1;
        *(s32 *)(ptr + 136) = ReadDeltaValue(a0, a1);
        *(s32 *)(ptr + 12) = *(s32 *)(ptr + 4);
        break;
    case 30:
        temp = ptr[40];
        ptr[22] = (u8)a2;
        if ((u32)temp < 127u) {
            ptr[40] = temp - 1;
            *(s32 *)(ptr + 136) = ReadDeltaValue(a0, a1);
            if (ptr[40] != 0) {
                *(s32 *)(ptr + 4) = *(s32 *)(ptr + 12);
            } else {
                ptr[16] = 0;
            }
        } else {
            ReadDeltaValue(a0, a1);
            *(s32 *)(ptr + 136) = 0;
            *(s32 *)(ptr + 4) = *(s32 *)(ptr + 12);
        }
        break;
    default:
        ptr[22] = (u8)a2;
        ptr[42] = ptr[42] + 1;
        *(s32 *)(ptr + 136) = ReadDeltaValue(a0, a1);
        break;
    }
}
