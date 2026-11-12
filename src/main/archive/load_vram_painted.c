/* Function: load_vram */
/* Address: 0x80066D38 */
/* Size: 1968 bytes (492 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_func_80066D38.py */
#include "types.h"

extern u8 byouki_kind;
extern s32 frame_counter;
extern u8 update_anim_state_tbl3;
extern u8 update_anim_state_tbl2;
extern u8 anim_fade;
extern u8 D_800BC0C8b;
extern u8 update_anim_state_tbl4;
extern s32 activeBuff;
extern u8 screen_bri;
extern u8 update_anim_state_tbl0[];
extern u8 update_anim_state_tbl1[];
extern u8 ot_buff0[];

extern s32 func_80066B5C(void);
extern s32 rand(void);
extern s32 rsin(s32);
extern s32 rcos(s32);
extern s32 prim_alloc(s32, s32);
extern void SetSprt(s32);
extern void SetSemiTrans(s32, s32);
extern s32 GetClut(void);
extern void AddPrim(u8 *, s32);
extern void goto_tpage(s32, s32, s32, s32, s32);
extern void func_80044568(s32, s32, s32, s32, u8 *);

void load_vram(s32 arg0) {
    u8 flags[10][8];
    s32 savedResult;
    s32 s0;
    s32 i, j;
    s32 entry;
    u8 *s8base;
    s32 v0val;

    savedResult = func_80066B5C();

    switch (byouki_kind & 0xF) {
    case 1:
        s0 = frame_counter;
        if ((s0 & 3) == 3) {
            update_anim_state_tbl3--;
        }
        v0val = 1;
        if (v0val == (s0 & 1)) {
            update_anim_state_tbl2++;
        }
        break;
    case 2:
        s0 = frame_counter;
        if ((s0 & 7) == 7) {
            update_anim_state_tbl3--;
        }
        if ((s0 & 3) == 3) {
            update_anim_state_tbl2++;
        }
        v0val = 1;
        break;
    case 3:
        s0 = frame_counter;
        if ((s0 & 0x15) == 0x15) {
            update_anim_state_tbl3--;
        }
        if ((s0 & 7) == 7) {
            update_anim_state_tbl2++;
        }
        v0val = 1;
        break;
    case 4:
        {
            s32 r;
            s0 = 16;
            r = rand();
            update_anim_state_tbl3 += (u8)(s0 - (r & 0x1F));
            r = rand();
            update_anim_state_tbl2 += (u8)(s0 - (r & 0x1F));
        }
        v0val = 1;
        break;
    case 5:
        {
            s32 r;
            s0 = frame_counter;
            r = rsin(s0);
            update_anim_state_tbl3 += (u8)(r / 32);
            s0 = frame_counter;
            r = rcos(s0);
            update_anim_state_tbl2 += (u8)(r / 32);
        }
        v0val = 1;
        break;
    case 0:
    default:
        s0 = frame_counter;
        v0val = 1;
        if (v0val == (s0 & 1)) {
            update_anim_state_tbl3--;
        }
        update_anim_state_tbl2++;
        break;
    }

    for (i = 0; i < 8; i++) {
        flags[0][i] = (u8)v0val;
        flags[1][i] = (u8)v0val;
        for (j = 2; j < 10; j += 4) {
            flags[j][i] = (u8)v0val;
            flags[j+1][i] = (u8)v0val;
            flags[j+2][i] = (u8)v0val;
            flags[j+3][i] = (u8)v0val;
        }
    }

    s8base = &ot_buff0[4480];
    if (arg0 == 0) {

    } else if (arg0 == 1) {
        i = 1;
        for (; i < 6; i++) {
            for (j = 1; j < 9; j += 4) {
                flags[j][i] = 0;
                flags[j+1][i] = 0;
                flags[j+2][i] = 0;
                flags[j+3][i] = 0;
            }
        }
    } else if (arg0 == 2) {
        for (i = 0; i < 6; i++) {
            flags[0][i] = 0;
            flags[1][i] = 0;
            for (j = 2; j < 10; j += 4) {
                flags[j][i] = 0;
                flags[j+1][i] = 0;
                flags[j+2][i] = 0;
                flags[j+3][i] = 0;
            }
        }
        flags[0][5] = 1;
        flags[9][5] = 1;
        flags[0][6] = 1;
        flags[9][6] = 1;
    }

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 10; j++) {
            if (flags[j][i] != 0) {
                s32 s4;
                s4 = (i << 5) + 240;
                s0 = j << 5;
                entry = prim_alloc(20, activeBuff);
                SetSprt(entry);
                SetSemiTrans(entry, 0);
                *(s16 *)(entry + 8) = (s16)s0;
                *(s16 *)(entry + 10) = (s16)(s4 - activeBuff * 240);
                *(u8 *)(entry + 12) = (u8)((update_anim_state_tbl2 & 0x1F) + 128);
                *(u8 *)(entry + 13) = (u8)((update_anim_state_tbl3 & 0x1F) + 128);
                *(s16 *)(entry + 18) = 32;
                *(s16 *)(entry + 16) = 32;
                *(u8 *)(entry + 4) = screen_bri;
                *(u8 *)(entry + 5) = screen_bri;
                *(u8 *)(entry + 6) = screen_bri;
                *(s16 *)(entry + 14) = (s16)GetClut();
                AddPrim(
                    s8base + ((u32)(activeBuff << 10)) + ((u32)update_anim_state_tbl4 << 2),
                    entry);
            }
        }
    }

    goto_tpage(960, 256, update_anim_state_tbl4, 0, 0);

    s0 = frame_counter;
    if ((s0 & 3) == 3) {
        u32 fadeVal;
        fadeVal = anim_fade;
        if (fadeVal >= 239U) {
            fadeVal = fadeVal + 1;
            if ((fadeVal & 0xFF) == 255) {
                u8 *t1base;
                savedResult++;
                t1base = &update_anim_state_tbl0[((savedResult + 3) & 3) << 5];
                anim_fade = (u8)fadeVal;
                func_80044568(160, 481, 16, 1, t1base);
                return;
            } else {
                anim_fade = (u8)fadeVal;
                if (fadeVal >= 240U) {
                    u8 *t1base;
                    u8 *src2;
                    u8 *dst;
                    s32 blendA;
                    s32 blendB;
                    s32 k;

                    t1base = &update_anim_state_tbl0[((savedResult + 3) & 3) << 5];
                    blendA = 255 - (s32)fadeVal;
                    blendB = (s32)fadeVal - 239;
                    src2 = &update_anim_state_tbl0[((savedResult + 4) & 3) << 5];
                    dst = update_anim_state_tbl1;
                    D_800BC0C8b = (u8)fadeVal;
                    for (k = 0; k < 8; k++) {
                        u32 v1val;

                        v1val = blendA * (*(u32 *)t1base & 0x1F) + blendB * (*(u32 *)src2 & 0x1F);
                        dst[0] = (u8)((v1val >> 4 & 0x1F) | (dst[0] & 0xFFE0));

                        v1val = blendA * ((*(u32 *)t1base << 22) >> 27) + blendB * ((*(u32 *)src2 << 22) >> 27);
                        *(u16 *)dst = (u16)(((v1val >> 4 & 0x1F) << 5 & 0x3E0) | (*(u16 *)dst & 0xFC1F));

                        v1val = blendA * ((*(u32 *)t1base << 17) >> 27) + blendB * ((*(u32 *)src2 << 17) >> 27);
                        dst[1] = (u8)(((v1val >> 4) << 2 & 0x7C) | (dst[1] & 0xFF83));

                        v1val = blendA * (*(u16 *)(t1base + 2) & 0x1F) + blendB * (*(u16 *)(src2 + 2) & 0x1F);
                        dst[2] = (u8)((v1val >> 4 & 0x1F) | (dst[2] & 0xFFE0));

                        v1val = blendA * ((*(u32 *)t1base << 6) >> 27) + blendB * ((*(u32 *)src2 << 6) >> 27);
                        *(u16 *)(dst + 2) = (u16)(((v1val >> 4 & 0x1F) << 5 & 0x3E0) | (*(u16 *)(dst + 2) & 0xFC1F));

                        v1val = blendA * ((*(u32 *)t1base << 1) >> 27) + blendB * ((*(u32 *)src2 << 1) >> 27);
                        dst[3] = (u8)(((v1val >> 4) << 2 & 0x7C) | (dst[3] & 0xFF83));

                        t1base += 4;
                        src2 += 4;
                        dst += 4;
                    }
                    func_80044568(160, 481, 16, 1, update_anim_state_tbl1);
                }
            }
        }
    }
}
