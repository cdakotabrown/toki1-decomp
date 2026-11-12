/* Function: SpuSetReverbModeParam */
/* Address: 0x80098800 */
/* Size: 1408 bytes (352 instructions) */
/* Region: Library (0x80086000+) */
/* PIPELINE: IDO 5.2 codegen assembled by ASPSX (@Compiler: ido52-gas). */
/* FALLTHRU alone 23 F14 Body 321).  Shipped gas config was `@Compiler: */
/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_NO_FALLTHRU_FILL=4,7,8,11,12,14,15,16,20,21,22,24 IDO52_FORCE_WEB_COLOR_NOSPLIT=2 IDO52_FORCE_WEB_COLOR=2:t0 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_NO_JUMP_MERGE=0,1 IDO52_AS1_FTF_BACKWARD=all IDO52_AS1_NO_PASS1_PROP=5 IDO52_UGEN_FORCE_TARGET_SAFE_DIV=1 IDO52_AS1_TIE_TAKE=38,40,111,116,176,298,327,351 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_instructions 0x264 0x268 */
/* @PostProc: swap_instructions 0x260 0x264 */
/* @PostProc: swap_instructions 0x25c 0x260 */
/* @PostProc: remap_stack_slots --map 20:44,28:48 */
#include "types.h"

typedef struct {
    u8 pad[388];
    s16 x;
    s16 y;
} CdVol;

extern s32 _SpuIsInAllocateArea_();
extern void _memcpy();
extern void _spu_ioctl();
extern void SpuClearReverbWorkArea();

extern s32 _spu_rev_startaddr[];
typedef struct {
    s32 w[17];
} RevParam;

extern RevParam _spu_rev_param[];
extern s32 _spu_fd;
extern s32 _spu_rev_offsetaddr;
static CdVol *D_800E39A8;

extern s32 libspu_rev_mode;
extern s16 libspu_rev_depth_left;
extern s16 libspu_rev_depth_right;
extern s32 libspu_rev_delay;
extern s32 libspu_rev_feedback;

s32 SpuSetReverbModeParam(s32 *cmd_ptr)
{
    s32 buf[17];
    s32 flags;
    s32 type;
    s32 use_flag;
    volatile s32 is_null;
    s32 tn;
    s32 local_68;
    s32 local_64;
    s32 local_60;
    s32 local_56;
    s32 local_52;
    s32 *table_ptr;
    CdVol *p;
    s32 v;
    s32 q;

    flags = cmd_ptr[0];
    use_flag = 0;
    if (flags == 0) {
        tn = 1;
    } else {
        tn = 0;
    }
    is_null = tn;
    local_60 = 0;
    local_56 = 0;
    local_52 = 0;
    buf[0] = 0;

    if (tn || (flags & 1)) {
        type = cmd_ptr[1];
        if (type & 0x100) {
            type &= ~0x100;
            use_flag = 1;
        }
        if (type >= 10 || type < 0) {
            return -1;
        }
        table_ptr = &_spu_rev_startaddr[type];
        if (_SpuIsInAllocateArea_(*table_ptr) != 0) {
            return -1;
        }
        libspu_rev_mode = type;
        local_60 = 1;
        _spu_rev_offsetaddr = *table_ptr;
        _memcpy(buf, &_spu_rev_param[type], 68);
        goto sw1;
    sw1_7:
        libspu_rev_feedback = 255;
        libspu_rev_delay = 255;
        goto sw1_end;
    sw1_8:
        libspu_rev_feedback = 0;
        libspu_rev_delay = 255;
        goto sw1_end;
    sw1:
        switch (libspu_rev_mode) {
        case 7: goto sw1_7;
        case 8: goto sw1_8;
        }
        libspu_rev_feedback = 0;
        libspu_rev_delay = 0;
    sw1_end:;
    }

    if (is_null || (flags & 8)) {
        goto sw2;
    sw2_78:
        local_56 = 1;
        if (local_60 == 0) {
            _memcpy(buf, &_spu_rev_param[libspu_rev_mode], 68);
            buf[0] = 0x0C011C00;
        }
        libspu_rev_delay = cmd_ptr[3];
        v = libspu_rev_delay;
        q = (v << 13) / 127;
        ((s16 *)buf)[12] = q - ((u16 *)buf)[2];
        q = (v << 12) / 127;
        ((s16 *)buf)[13] = q - ((u16 *)buf)[3];
        ((s16 *)buf)[14] = ((u16 *)buf)[15] + (0, q);
        ((s16 *)buf)[18] = ((u16 *)buf)[19] + (0, q);
        ((s16 *)buf)[28] = ((u16 *)buf)[30] + (0, q);
        ((s16 *)buf)[29] = ((u16 *)buf)[31] + (0, q);
        goto sw2_end;
    sw2:
        switch (libspu_rev_mode) {
        case 7: goto sw2_78;
        case 8: goto sw2_78;
        }
        libspu_rev_delay = 0;
    sw2_end:;
    }

    if (is_null || (flags & 0x10)) {
        goto sw3;
    sw3_78:
        local_52 = 1;
        if (local_60 == 0) {
            if (local_56 == 0) {
                _memcpy(buf, &_spu_rev_param[libspu_rev_mode], 68);
                buf[0] = 0x80;
            } else {
                buf[0] |= 0x80;
            }
        }
        libspu_rev_feedback = cmd_ptr[4];
        ((s16 *)buf)[9] = (libspu_rev_feedback * 0x8100) / 127;
        goto sw3_end;
    sw3:
        switch (libspu_rev_mode) {
        case 7: goto sw3_78;
        case 8: goto sw3_78;
        }
        libspu_rev_feedback = 0;
    sw3_end:;
    }

    if (local_60) {
        _spu_ioctl(_spu_fd, 106, &local_64);
        if (local_64) {
            local_68 = 0;
            _spu_ioctl(_spu_fd, 96, &local_68);
        }
    }

    if (local_60 == 0) {
        if (!(is_null || (flags & 2))) {
            p = D_800E39A8;
        } else {
            p = D_800E39A8;
            p->x = ((s16 *)cmd_ptr)[4];
            libspu_rev_depth_left = ((s16 *)cmd_ptr)[4];
        }
        if (is_null || (flags & 4)) {
            p->y = ((s16 *)cmd_ptr)[5];
            libspu_rev_depth_right = ((s16 *)cmd_ptr)[5];
        }
    } else {
        p = D_800E39A8;
        p->x = 0;
        p->y = 0;
        libspu_rev_depth_left = 0;
        libspu_rev_depth_right = 0;
    }

    if (local_60 || local_56 || local_52) {
        _spu_ioctl(_spu_fd, 99, buf);
    }
    if (use_flag) {
        SpuClearReverbWorkArea(libspu_rev_mode);
    }
    if (local_60) {
        _spu_ioctl(_spu_fd, 97, &_spu_rev_offsetaddr);
    }
    if (local_60 && local_64) {
        _spu_ioctl(_spu_fd, 96, &local_64);
    }
    return 0;
}
