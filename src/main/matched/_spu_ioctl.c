/* Address: 0x8009607C */
/* Size: 2460 bytes (615 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 codegen, NATIVE as1 since */
/* 2026-09-25 (was ido52-gas) */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_NO_JUMP_MERGE=all IDO52_FORCE_WEB_COLOR=18:v1 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_KEEP_ANDI=all IDO52_AS1_NO_FALLTHRU_FILL=8,13,52 IDO52_AS1_FORCE_BOPT=3,5,7 IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_FTF_BACKWARD=all IDO52_AS1_DIV_MACRO=aspsx IDO52_AS1_TIE_REJECT=19,79,92,34,35,48,49 IDO52_AS1_FTF_FORCE=occ:22,44 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern volatile u16 * volatile _spu_RXX;
static s32 tcount;
extern u16 spu_tsa;
extern s32 spu_transMode;
extern char libspu_ioctl_irq_off_timeout_fmt[];
extern char libspu_ioctl_irq_off_wait_str[];
extern char libspu_ioctl_irq_on_timeout_fmt[];
extern char libspu_ioctl_irq_on_wait_str[];

extern void printf(char *, char *);
extern void _SpuCallback(void *);
extern void _spu_setReverbAttr(void *);
extern void _spu_setCommonAttr(void *);
extern void _spu_getCommonAttr(void *);
extern void _spu_r_(void *, s32, s32);

s32 _spu_ioctl(s32 ctx, u32 cmd, u32 *p)
{
    volatile u16 *r;
    s32 shift;
    s32 x;
    u16 v;

    r = _spu_RXX;
    x = r[0xD7] & 0x7FF;
    shift = (r[0xD6] & 0xE) >> 1;

    goto dispatch;
c1:
        if (p == 0) {
            r[0xD5] |= 0x4000;
        } else {
            r[0xD5] &= 0xBFFF;
        }
        goto end0;
c2:
        if (r[0xD5] & 0x4000) {
            *p = 0;
        } else {
            *p = 1;
        }
        goto end0;
c16:
        if (p == 0) {
            v = r[0xD5];
            v &= 0xFFBF;
            r[0xD5] = v;
            tcount = 0;
            if ((r[0xD5] & 0x40) == 0) {
            } else {
                do {
                    tcount++;
                    if (tcount >= 5001) {
                        printf(libspu_ioctl_irq_off_timeout_fmt, libspu_ioctl_irq_off_wait_str);
                        return 0;
                    }
                } while (r[0xD5] & 0x40);
            }
        } else {
            v = r[0xD5];
            v |= 0x40;
            r[0xD5] = v;
            tcount = 0;
            if ((r[0xD5] & 0x40) != 0) {
            } else {
                do {
                    tcount++;
                    if (tcount >= 5001) {
                        printf(libspu_ioctl_irq_on_timeout_fmt, libspu_ioctl_irq_on_wait_str);
                        return 0;
                    }
                } while (!(r[0xD5] & 0x40));
            }
        }
        goto end0;
c22:
        if (r[0xD5] & 0x40) {
            *p = 1;
        } else {
            *p = 0;
        }
        goto end0;
c19:
        x = *p;
        if (shift != 0) {
            if (x % shift != 0) {
                x += shift;
            }
        }
        r[0xD2] = (x >> (shift + 1)) & 0xFFFF;
        goto end0;
c20:
        v = r[0xD2] & 0xFFFF;
        *p = v << (shift + 1);
        goto end0;
c21:
        _SpuCallback(p);
        return 0;
c43:
        x = *p;
        if (shift != 0) {
            if (x % shift != 0) {
                x += shift;
            }
        }
        spu_tsa = (x >> (shift + 1)) & 0xFFFF;
        goto end0;
c45:
        *p = spu_tsa << (shift + 1);
        goto end0;
c33:
c46:
        spu_transMode = *p;
        goto end0;
c47:
        *p = spu_transMode;
        goto end0;
c64:
        v = r[0xC8];
        v |= (u16)(*p & 0xFFFF);
        r[0xC8] = v;
        v = r[0xC9];
        v |= (u16)((*p >> 16) & 0xFF);
        r[0xC9] = v;
        goto end0;
c65:
        *p = (r[0xC8] & 0xFFFF) | ((r[0xC9] & 0xFF) << 16);
        goto end0;
c66:
        v = r[0xC8];
        v &= ~(u16)(*p & 0xFFFF);
        r[0xC8] = v;
        v = r[0xC9];
        v &= ~(u16)((*p >> 16) & 0xFF);
        r[0xC9] = v;
        goto end0;
c80:
        v = r[0xD5];
        v &= 0xC0FF;
        v |= (*(u16 *)p & 0x3F) << 8;
        r[0xD5] = v;
        goto end0;
c81:
        *(u16 *)p = (r[0xD5] & 0x3F00) >> 8;
        goto end0;
c82:
        r[0xCA] = (u16)(*p & 0xFFFF) | r[0xCA];
        r[0xCB] = (u16)((*p >> 16) & 0xFF) | r[0xCB];
        goto end0;
c83:
        *p = (r[0xCA] & 0xFFFF) | ((r[0xCB] & 0xFF) << 16);
        goto end0;
c84:
        r[0xCA] = (u16)~(*p & 0xFFFF) & r[0xCA];
        r[0xCB] = (u16)~((*p >> 16) & 0xFF) & r[0xCB];
        goto end0;
c96:
        v = r[0xD5];
        v &= 0xFF7F;
        v |= (*(u16 *)p & 1) << 7;
        r[0xD5] = v;
        goto end0;
c106:
        v = r[0xD5];
        if (v & 0x80) {
            *p = 1;
        } else {
            *p = 0;
        }
        goto end0;
c97:
        r[0xD1] = *(u16 *)p & 0xFFFF;
        goto end0;
c104:
        x = *p;
        if (shift != 0) {
            if (x % shift != 0) {
                x += shift;
            }
        }
        r[0xD1] = (x >> (shift + 1)) & 0xFFFF;
        goto end0;
c105:
        v = r[0xD1] & 0xFFFF;
        *p = v << (shift + 1);
        goto end0;
c99:
        _spu_setReverbAttr(p);
        return 0;
c101:
        v = r[0xCC];
        v |= (u16)(*p & 0xFFFF);
        r[0xCC] = v;
        v = r[0xCD];
        v |= (u16)((*p >> 16) & 0xFF);
        r[0xCD] = v;
        goto end0;
c102:
        *p = (r[0xCC] & 0xFFFF) | ((r[0xCD] & 0xFF) << 16);
        goto end0;
c103:
        v = r[0xCC];
        v &= (u16)~(*p & 0xFFFF);
        r[0xCC] = v;
        v = r[0xCD];
        v &= (u16)~((*p >> 16) & 0xFF);
        r[0xCD] = v;
        goto end0;
c256:
        _spu_setCommonAttr(p);
        return 0;
c257:
        _spu_getCommonAttr(p);
        return 0;
c516:
        r[0xC4] = *p & 0xFFFF;
        r[0xC5] = (*p >> 16) & 0xFF;
        goto end0;
c515:
        r[0xC6] = *p & 0xFFFF;
        r[0xC7] = (*p >> 16) & 0xFF;
        goto end0;
c528:
        _spu_r_(p, 0, 64);
        r = _spu_RXX;
        if (r[0xD7] & 0x800) {
            return 0;
        }
        return 1;
c530:
        _spu_r_(p, 0, 32);
        r = _spu_RXX;
        if (r[0xD7] & 0x800) {
            return 0;
        }
        return 1;
c531:
        _spu_r_((u8 *)p + 2048, 256, 32);
        r = _spu_RXX;
        if (r[0xD7] & 0x800) {
            return 0;
        }
        return 1;
dispatch:
    if (cmd == 96) goto c96;
    if (cmd >= 97) goto t97;
    if (cmd == 64) goto c64;
    if (cmd >= 65) goto t65;
    if (cmd == 33) goto c33;
    if (cmd >= 34) goto t34;
    if (cmd == 19) goto c19;
    if (cmd >= 20) goto t20;
    if (cmd == 2) goto c2;
    if (cmd >= 3) goto t3;
    if (cmd == 1) goto c1;
    return 0;
t3:
    if (cmd == 16) goto c16;
    return 0;
t20:
    if (cmd == 21) goto c21;
    if (cmd >= 22) goto t22;
    if (cmd == 20) goto c20;
    return 0;
t22:
    if (cmd == 22) goto c22;
    return 0;
t34:
    if (cmd == 46) goto c46;
    if (cmd >= 47) goto t47;
    if (cmd == 45) goto c45;
    if (cmd >= 46) goto end0;
    if (cmd == 43) goto c43;
    return 0;
t47:
    if (cmd == 47) goto c47;
    return 0;
t65:
    if (cmd == 81) goto c81;
    if (cmd >= 82) goto t82;
    if (cmd == 66) goto c66;
    if (cmd >= 67) goto t67;
    if (cmd == 65) goto c65;
    return 0;
t67:
    if (cmd == 80) goto c80;
    return 0;
t82:
    if (cmd == 83) goto c83;
    if (cmd >= 84) goto t84;
    if (cmd == 82) goto c82;
    return 0;
t84:
    if (cmd == 84) goto c84;
    return 0;
t97:
    if (cmd == 106) goto c106;
    if (cmd >= 107) goto t107;
    if (cmd == 102) goto c102;
    if (cmd >= 103) goto t103;
    if (cmd == 99) goto c99;
    if (cmd >= 100) goto t100;
    if (cmd == 97) goto c97;
    return 0;
t100:
    if (cmd == 101) goto c101;
    return 0;
t103:
    if (cmd == 104) goto c104;
    if (cmd >= 105) goto t105;
    if (cmd == 103) goto c103;
    return 0;
t105:
    if (cmd == 105) goto c105;
    return 0;
t107:
    if (cmd == 516) goto c516;
    if (cmd >= 517) goto t517;
    if (cmd == 257) goto c257;
    if (cmd >= 258) goto t258;
    if (cmd == 256) goto c256;
    return 0;
t258:
    if (cmd == 515) goto c515;
    return 0;
t517:
    if (cmd == 530) goto c530;
    if (cmd >= 531) goto t531;
    if (cmd == 528) goto c528;
    return 0;
t531:
    if (cmd == 531) goto c531;
end0:
    return 0;
}
