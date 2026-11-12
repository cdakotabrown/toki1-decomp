/* Function: SD_BufferUpdate */
/* Address: 0x80088F90 */
/* Size: 724 bytes (181 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 --fill-epilogue */
/* @PostProc: custom_script custom_SD_BufferUpdate.py */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

typedef signed char schar;

extern s32 CD_status1;
extern s32 libcd_media_gen_seen;
extern s32 CD_debug;

typedef struct {
    s32 words[6];
} SdBufEntry;

extern SdBufEntry D_800F92F0;

extern s32 CD_newmedia();
extern s32 CD_searchdir(s32, schar *);
extern s32 CD_cachefile(s32);
extern void printf(schar *, ...);

s32 SD_BufferUpdate(s32 *arg0, schar *arg1, s32 arg2) {
    schar nameBuf[24];
    schar *ptr;
    schar *dst;
    s32 i;
    s32 result;
    SdBufEntry *base;
    schar *namePtr;
    s32 backslash;
    s32 minus1;
    schar sc;
    u8 uc;

    if (libcd_media_gen_seen != CD_status1) {
        do {
            if (CD_newmedia() != 0) {
                break;
            }
        } while (1);
    }
    libcd_media_gen_seen = CD_status1;

    if (*arg1 != '\\') {
        return 0;
    }

    nameBuf[0] = 0;
    ptr = arg1;
    i = 0;
    backslash = '\\';
    minus1 = -1;

outer_loop:
    sc = *ptr;
    uc = *(u8 *)ptr;
    dst = nameBuf;
    if (sc == backslash) {
        goto do_backslash;
    }

inner_top:
    if (uc == 0) {
        goto end_parse;
    }
    *dst = uc;
    sc = ptr[1];
    uc = ((u8 *)ptr)[1];
    ptr++;
    if (sc == backslash) {
        goto do_backslash_mid;
    }
    dst++;
    goto inner_top;

do_backslash_mid:
    sc = *ptr;
do_backslash:
    if (sc == 0) {
        goto end_parse;
    }
    ptr++;
    *dst = 0;
    result = CD_searchdir(1, nameBuf);
    if (result == minus1) {
        nameBuf[0] = 0;
        goto end_parse;
    }
    i++;
    if (i < 8) {
        goto outer_loop;
    }

end_parse:
    if (i >= 8) {
        if (CD_debug > 0) {
            printf((schar *)0x800B8C90, arg1, i);
        }
        return 0;
    }

    if (nameBuf[0] == 0) {
        if (CD_debug > 0) {
            printf((schar *)0x800B8CAC, arg1);
        }
        return 0;
    }

    *dst = 0;
    result = CD_cachefile(result);
    if (result == 0) {
        if (CD_debug > 0) {
            printf((schar *)0x800B8CC4);
        }
        return 0;
    }

    if (CD_debug >= 2) {
        printf((schar *)0x800B8CE0, nameBuf);
    }

    base = &D_800F92F0;
    namePtr = (schar *)base + 8;
    i = 0;

    while (i < 64) {
        if (*(u8 *)namePtr == 0) {
            break;
        }
        if (CD_newmedia(namePtr, nameBuf) != 0) {
            s32 w0, w1, w2, w3;
            if (CD_debug >= 2) {
                printf((schar *)0x800B8D00, nameBuf);
            }
            w0 = base->words[0];
            w1 = base->words[1];
            w2 = base->words[2];
            w3 = base->words[3];
            arg0[0] = w0;
            arg0[1] = w1;
            arg0[2] = w2;
            arg0[3] = w3;
            w1 = base->words[5];
            w0 = base->words[4];
            arg0[5] = w1;
            arg0[4] = w0;
            return (s32)base;
        }
        i++;
        base++;
        namePtr += 24;
    }

    if (CD_debug > 0) {
        printf((schar *)0x800B8D0C, nameBuf);
    }
    return 0;
}
