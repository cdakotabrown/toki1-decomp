/* Function: date_alubum_para_set */
/* Address: 0x80094078 */
/* Size: 1308 bytes (327 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 24 --new 40 --shift-ra */
/* @PostProc: strip_trailing_instructions --count 1 --nop-only */
/* @PostProc: custom_script custom_func_80094078.py */
#include "types.h"

extern void SsUtGetVagAtr(s16 a0, s16 a1, s16 a2, u8 *data);
extern void SsUtSetVagAtr(s16 a0, s16 a1, s16 a2, u8 *data);
extern void SsUtResolveADSR(u16 a0, u16 a1, u8 *data);
extern void SsUtBuildADSR(u8 *a0, u8 *a1, u8 *a2);
extern void SsUtReverbOff(void);
extern void SsUtReverbOn(void);
extern void SsUtSetReverbType(s16 a0);
extern void SsUtSetReverbDepth(s16 a0, s16 a1);
extern void SsUtSetReverbFeedback(s16 a0);
extern void SsUtSetReverbDelay(s16 a0);

void date_alubum_para_set(s16 a0, s16 a1, s16 a2, s32 a3) {
    u8 *buf;
    buf = (u8 *)&a3;
    SsUtGetVagAtr(a0, a1, a2, buf);

    switch (*(s16 *)(buf + 52)) {
        case 0:
            buf[0] = buf[56];
            SsUtSetVagAtr(a0, a1, a2, buf);
            break;
        case 1:
            buf[1] = buf[56];
            SsUtSetVagAtr(a0, a1, a2, buf);
            if (buf[56] == 0) {
                SsUtReverbOff();
            } else if (buf[56] == 1) {
            } else if (buf[56] == 2) {
            } else if (buf[56] == 3) {
            } else if (buf[56] == 4) {
                SsUtReverbOn();
            }
            break;
        case 2:
            buf[6] = buf[56];
            SsUtSetVagAtr(a0, a1, a2, buf);
            break;
        case 3:
            buf[7] = buf[56];
            SsUtSetVagAtr(a0, a1, a2, buf);
            break;
        case 4:
            SsUtResolveADSR(*(u16 *)(buf + 16), *(u16 *)(buf + 18), buf + 32);
            *(s16 *)(buf + 42) = 0;
            *(s16 *)(buf + 32) = (s16)buf[56];
            SsUtBuildADSR(buf + 32, buf + 16, buf + 18);
            SsUtSetVagAtr(a0, a1, a2, buf);
            break;
        case 5:
            SsUtResolveADSR(*(u16 *)(buf + 16), *(u16 *)(buf + 18), buf + 32);
            *(s16 *)(buf + 42) = 1;
            *(s16 *)(buf + 32) = (s16)buf[56];
            SsUtBuildADSR(buf + 32, buf + 16, buf + 18);
            SsUtSetVagAtr(a0, a1, a2, buf);
            break;
        case 6:
            SsUtResolveADSR(*(u16 *)(buf + 16), *(u16 *)(buf + 18), buf + 32);
            *(s16 *)(buf + 34) = (s16)buf[56];
            SsUtBuildADSR(buf + 32, buf + 16, buf + 18);
            SsUtSetVagAtr(a0, a1, a2, buf);
            break;
        case 7:
            SsUtResolveADSR(*(u16 *)(buf + 16), *(u16 *)(buf + 18), buf + 32);
            *(s16 *)(buf + 36) = (s16)buf[56];
            SsUtBuildADSR(buf + 32, buf + 16, buf + 18);
            SsUtSetVagAtr(a0, a1, a2, buf);
            break;
        case 8:
            SsUtResolveADSR(*(u16 *)(buf + 16), *(u16 *)(buf + 18), buf + 32);
            *(s16 *)(buf + 44) = 0;
            *(s16 *)(buf + 38) = (s16)buf[56];
            SsUtBuildADSR(buf + 32, buf + 16, buf + 18);
            SsUtSetVagAtr(a0, a1, a2, buf);
            break;
        case 9:
            SsUtResolveADSR(*(u16 *)(buf + 16), *(u16 *)(buf + 18), buf + 32);
            *(s16 *)(buf + 44) = 1;
            *(s16 *)(buf + 38) = (s16)buf[56];
            SsUtBuildADSR(buf + 32, buf + 16, buf + 18);
            SsUtSetVagAtr(a0, a1, a2, buf);
            break;
        case 10:
            SsUtResolveADSR(*(u16 *)(buf + 16), *(u16 *)(buf + 18), buf + 32);
            *(s16 *)(buf + 46) = 0;
            *(s16 *)(buf + 40) = (s16)buf[56];
            SsUtBuildADSR(buf + 32, buf + 16, buf + 18);
            SsUtSetVagAtr(a0, a1, a2, buf);
            break;
        case 11:
            SsUtResolveADSR(*(u16 *)(buf + 16), *(u16 *)(buf + 18), buf + 32);
            *(s16 *)(buf + 46) = 1;
            *(s16 *)(buf + 40) = (s16)buf[56];
            SsUtBuildADSR(buf + 32, buf + 16, buf + 18);
            SsUtSetVagAtr(a0, a1, a2, buf);
            break;
        case 12:
            SsUtResolveADSR(*(u16 *)(buf + 16), *(u16 *)(buf + 18), buf + 32);
            {
                u32 val12 = buf[56];
                if (val12 != 0) {
                    if (val12 < 64) {
                        *(s16 *)(buf + 48) = 0;
                    } else if (val12 < 128) {
                        *(s16 *)(buf + 48) = 1;
                    }
                }
            }
            SsUtBuildADSR(buf + 32, buf + 16, buf + 18);
            SsUtSetVagAtr(a0, a1, a2, buf);
            break;
        case 13:
            SsUtResolveADSR(*(u16 *)(buf + 16), *(u16 *)(buf + 18), buf + 32);
            buf[9] = buf[56];
            SsUtBuildADSR(buf + 32, buf + 16, buf + 18);
            SsUtSetVagAtr(a0, a1, a2, buf);
            break;
        case 14:
            SsUtResolveADSR(*(u16 *)(buf + 16), *(u16 *)(buf + 18), buf + 32);
            buf[10] = buf[56];
            SsUtBuildADSR(buf + 32, buf + 16, buf + 18);
            SsUtSetVagAtr(a0, a1, a2, buf);
            break;
        case 15:
            SsUtSetReverbType((s16)buf[56]);
            break;
        case 16:
            SsUtSetReverbDepth((s16)buf[56], (s16)buf[56]);
            break;
        case 17:
            SsUtSetReverbFeedback((s16)buf[56]);
            break;
        case 18:
            SsUtSetReverbDelay((s16)buf[56]);
            break;
        case 19:
            SsUtSetReverbDelay((s16)buf[56]);
            break;
        case 20:
        case 21:
        case 22:
            break;
    }
}
