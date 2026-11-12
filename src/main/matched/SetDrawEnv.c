/* Address: 0x800A0DF8 */
/* Size: 736 bytes (184 instructions) */
/* Region: Library (IDO 5.2, -O2 -- the same TU as StoreImage */
/* @CompileFlags: -O2 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @CompileEnv: IDO52_AS1_NO_ADDIU_COMBINE=all IDO52_AS1_KEEP_ANDI=occ:2 IDO52_AS1_NO_PASS1_PROP=8 IDO52_AS1_NO_LA_PULL=0,1,2,5 IDO52_AS1_KEEP_DEAD_RELOAD=0,1 IDO52_LDA_NOINREG_CLASSES=1 */
#include "types.h"

typedef struct {
    s16 x, y, w, h;
} Rect;

typedef struct {
    volatile s16 x, y, w, h;
} VRect;

typedef struct {
    u32 addr : 24;
    u32 len : 8;
} PTag;

typedef struct {
    PTag tag;
    u32 code[15];
} DrEnv;

typedef struct {
    Rect clip;
    s16 ofs[2];
    Rect tw;
    u16 tpage;
    u8 dtd;
    u8 dfe;
    u8 isbg;
    u8 r0, g0, b0;
} DrawEnv;

extern s16 D_800EBD64;
extern s16 D_800EBD66;

extern u32 get_cs(s32 x, s32 y);
extern u32 get_ce(s32 x, s32 y);
extern u32 get_ofs(s32 x, s32 y);
extern u32 get_mode(s32 dfe, s32 dtd, s32 tpage);
extern u32 get_tw(Rect *tw);

void SetDrawEnv(DrEnv *dr_env, DrawEnv *env)
{
    u16 w, h;
    DrEnv *dr;
    s32 len;
    VRect rect;
    s32 t;

    len = 1;
    dr = dr_env;
    ((u32 *)&dr->code[-1])[len++] = get_cs(env->clip.x, env->clip.y);
    ((u32 *)&dr->code[-1])[len++] = get_ce(env->clip.w + env->clip.x - 1,
                          env->clip.y + env->clip.h - 1);
    ((u32 *)&dr->code[-1])[len++] = get_ofs(env->ofs[0], env->ofs[1]);
    ((u32 *)&dr->code[-1])[len++] = get_mode(env->dfe, env->dtd, env->tpage);
    ((u32 *)&dr->code[-1])[len++] = get_tw(&env->tw);
    ((u32 *)&dr->code[-1])[len++] = 0xE6000000;
    if (env->isbg) {
        rect.x = env->clip.x;
        rect.y = env->clip.y;
        rect.w = env->clip.w;
        rect.h = env->clip.h;
        if (((Rect *)&rect)->w < 0) {
            rect.w = 0;
        } else {
            t = (D_800EBD64 < rect.w) ? D_800EBD64 : rect.w;
            rect.w = t;
        }
        if (rect.h < 0) {
            rect.h = 0;
        } else {
            t = (D_800EBD66 < rect.h) ? D_800EBD66 : rect.h;
            rect.h = t;
        }
        if (rect.x & 0x3F || rect.w & 0x3F) {
            ((Rect *)&rect)->x -= env->ofs[0];
            rect.y -= env->ofs[1];
            ((u32 *)&dr->code[-1])[len++] = 0x60000000 | (env->b0 << 16) | (env->g0 << 8) | env->r0;
            ((u32 *)&dr->code[-1])[len++] = *(u32 *)&rect.x;
            ((u32 *)&dr->code[-1])[len++] = *(u32 *)&rect.w;
            ((Rect *)&rect)->x += env->ofs[0];
            rect.y += env->ofs[1];
        } else {
            ((u32 *)&dr->code[-1])[len++] = 0x02000000 | (env->b0 << 16) | (env->g0 << 8) | env->r0;
            ((u32 *)&dr->code[-1])[len++] = *(u32 *)&rect.x;
            ((u32 *)&dr->code[-1])[len++] = *(u32 *)&rect.w;
        }
    }
    t = *(u32 *)dr;
    *(u32 *)dr = ((((len - 1) & 0xFF) ^ ((u32)t >> 24)) << 24) ^ t;
}
