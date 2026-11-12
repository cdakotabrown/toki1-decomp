/* Address: 0x800B48D0 */
/* Size: 3460 bytes (865 instructions) */
/* Region: Library (0x80086000-0x800BFFFF), IDO 5.2 at -O1 (every local is */
/* @SymbolStart: 0x800B48D0 */
/* @SymbolSize: 3456 */
/* @CompileFlags: -Wo,-O1 */
/* @CompileEnv: IDO52_CFE_SWITCH_CHAIN=all IDO52_UGEN_NO_MOVE_NOALIAS=occ:0 IDO52_CFE_BITFIELD_WORD=all IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_NO_BRANCH_FOLD=all IDO52_AS1_NO_FALLTHRU_FILL=0,2,15,16,19,21,22,28,30,34,38,40 IDO52_AS1_NO_PASS1_PROP=10,80,555,563,582,601 IDO52_UGEN_TEMP_REG=8:t2,16:t2,51:t5,70:t1,71:t3,75:t8,151:t1,161:t2,196:t3,214:t5,265:t9,276:t1,290:t7,300:t2,301:t3,348:t5,363:t0,365:t0 IDO52_AS1_FTF_BACKWARD=occ:3,4,5,8,11,12 IDO52_AS1_TIE_TAKE=8,15,21,193 IDO52_AS1_TIE_REJECT=165,215,78,100 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

typedef struct {
    u32 left:1;
    u32 plus:1;
    u32 alt:1;
    u32 zero:1;
    u32 haveprec:1;
    u32 isshort:1;
    u32 islong:1;
    u32 isL:1;
    s8  sign;
    s32 width;
    s32 prec;
} FmtSpec;

extern FmtSpec libapi_sprintf_default_spec;
extern s8 libapi_sprintf_hex_upper_tbl[];
extern s8 libapi_sprintf_hex_lower_tbl[];

extern s32 strlen(s8 *s);
extern s8 *memchr(s8 *s, s32 c, s32 n);
extern void memmove(char *d, s8 *s, s32 n);

s32 sprintf(dst, fmt, va_alist)
char *dst;
s8 *fmt;
s32 va_alist;
{
    s32 c;
    s32 n;
    s32 unused;
    s32 len;
    u32 val;
    s8 *p;
    s8 buf[512];
    s8 *digits;
    s8 *tmp;
    FmtSpec f;
    s32 *ap;

    len = 0;
    ap = &va_alist;
    for (c = *fmt; c != 0; c = *++fmt) {
        if (c != '%') goto literal;
        f = libapi_sprintf_default_spec;
        for (;;) {
            c = *++fmt;
            if (c == '-') { f.left = 1; continue; }
            if (c == '+') { f.plus = 1; continue; }
            if (c == ' ') { f.sign = ' '; continue; }
            if (c == '#') { f.alt = 1; continue; }
            if (c == '0') { f.zero = 1; continue; }
            break;
        }
        if (c == '*') {
            f.width = *ap++;
            if (f.width < 0) {
                f.left = 1;
                f.width = -f.width;
            }
            c = *++fmt;
        } else {
            while (c >= '0' && c <= '9') {
                f.width = f.width * 10 + c - '0';
                c = *++fmt;
            }
        }
        if (c == '.') {
            c = *++fmt;
            if (c == '*') {
                f.prec = *ap++;
                c = *++fmt;
            } else {
                while (c >= '0' && c <= '9') {
                    f.prec = f.prec * 10 + c - '0';
                    c = *++fmt;
                }
            }
            if (f.prec >= 0) f.haveprec = 1;
        }
        p = &buf[512];
        if (f.left) f.zero = 0;
again:
        switch (c) {
        case 'h':
            f.isshort = 1;
            c = *++fmt;
            goto again;
        case 'l':
            f.islong = 1;
            c = *++fmt;
            goto again;
        case 'L':
            f.isL = 1;
            c = *++fmt;
            goto again;
        case 'd':
        case 'i':
            if (f.islong) val = *ap++;
            else val = *ap++;
            if (f.isshort) val = (s16)val;
            if ((s32)val < 0) {
                val = -(s32)val;
                f.sign = '-';
            } else if (f.plus) {
                f.sign = '+';
            }
            goto dec;
        case 'u':
            if (f.islong) val = *ap++;
            else val = *ap++;
            if (f.isshort) val = (u16)val;
            f.sign = 0;
dec:
            if (!f.haveprec) {
                if (f.zero) {
                    f.prec = f.width;
                    if (f.sign) f.prec = f.width - 1;
                }
                if (f.prec <= 0) f.prec = 1;
            }
            n = 0;
            while (val) {
                *--p = val % 10 + '0';
                val = val / 10;
                n++;
            }
            while (n < f.prec) {
                *--p = '0';
                n++;
            }
            if (f.sign) {
                *--p = f.sign;
                n++;
            }
            break;
        case 'o':
            if (f.islong) val = *ap++;
            else val = *ap++;
            if (f.isshort) val = (u16)val;
            if (!f.haveprec) {
                if (f.zero) f.prec = f.width;
                if (f.prec <= 0) f.prec = 1;
            }
            n = 0;
            while (val) {
                *--p = (val & 7) + '0';
                val = val >> 3;
                n++;
            }
            if (f.alt) {
                if (n != 0) {
                    if (*p != '0') {
                        *--p = '0';
                        n++;
                    }
                }
            }
            while (n < f.prec) {
                *--p = '0';
                n++;
            }
            break;
        case 'p':
            f.haveprec = f.islong = 1;
            f.prec = 8;
        case 'X':
            digits = libapi_sprintf_hex_upper_tbl;
            goto hex;
        case 'x':
            digits = libapi_sprintf_hex_lower_tbl;
hex:
            if (f.islong) val = *ap++;
            else val = *ap++;
            if (f.isshort) val = (u16)val;
            if (!f.haveprec) {
                if (f.zero) {
                    f.prec = f.width;
                    if (f.alt) f.prec = f.width - 2;
                }
                if (f.prec <= 0) f.prec = 1;
            }
            n = 0;
            while (val) {
                *--p = *(s8 *)((u32)digits + (val & 15));
                val = val >> 4;
                n++;
            }
            while (n < f.prec) {
                *--p = '0';
                n++;
            }
            if (f.alt) {
                *--p = c;
                *--p = '0';
                n += 2;
            }
            break;
        case 'c':
            *--p = *ap;
            n = 1;
            ap++;
            break;
        case 's':
            p = (s8 *)*ap++;
            if (f.alt) {
                n = *(u8 *)p;
                p++;
                if (f.haveprec) {
                    if (n > f.prec) n = f.prec;
                }
            } else if (!f.haveprec) {
                n = strlen(p);
            } else {
                tmp = memchr(p, 0, f.prec);
                if (tmp) n = tmp - p;
                else n = f.prec;
            }
            break;
        case 'n':
            p = (s8 *)*ap++;
            if (f.isshort) *(s16 *)p = len;
            else if (f.islong) *(s32 *)p = len;
            else *(s32 *)p = len;
            continue;
        default:
            if (c != '%') goto done;
literal:
            dst[len] = c;
            len++;
            continue;
        }
        if (n < f.width) {
            if (!f.left) {
                do {
                    dst[len] = ' ';
                    len++;
                    f.width--;
                } while (n < f.width);
            }
        }
        memmove(dst + len, p, n);
        len += n;
        while (n < f.width) {
            dst[len] = ' ';
            len++;
            n++;
        }
    }
done:
    dst[len] = 0;
    return len;
}
