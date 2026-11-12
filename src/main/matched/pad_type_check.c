/* Address: 0x80042D58 (true start 0x80042D48: the pad1_buf / pad_type */
/* Size: 472 bytes (118 instructions) over the corrected window */
/* Region: Game (IDO 5.2) */
/* @SymbolStart: 0x80042D48 */
/* @SymbolSize: 472 */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all IDO52_FORCE_WEB_COLOR=6:v1,4:a0,21:a1 IDO52_AS1_TIE_REJECT=4 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_branch_operands --offsets 0x6C */
#include "types.h"

extern void InitPAD(void *a, s32 b, void *c, s32 d);
extern void StartPAD(void);
extern void ChangeClearPAD(s32 val);
extern void InitMouse(void *a, void *b);
extern void SenseMouse(s32 a, s32 b);
extern void SetMouse(s32 a, s32 b, s32 c);
extern void RangeMouse(s32 a, s32 b, s32 c, s32 d);

extern u8 pad1_buf;
extern u8 pad1_buf_1;
extern volatile u8 pad_type;
extern u8 menu_mode;
extern u8 card_busy;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s16 mouse_x;
extern s16 mouse_y;
extern s32 pad_data;
extern s32 pad_prev;
extern s32 pad_trig;
extern u8 pad2_buf;

s32 pad_type_check(void)
{
    s32 inputState;
    s32 limit;
    u8 newState;
    s32 oldState;
    u8 tmp;

    inputState = pad1_buf;
    newState = pad_type;
    oldState = newState;

    if (inputState == 0) {
        tmp = pad1_buf_1;
        if ((u32)((tmp & 0xF) * 2) < 65u) {
            if ((tmp & 0xF0) == 0x10) {
                newState = 0;
                menu_mode = 0;
            } else {
                newState = 1;
            }
        }
        limit = 255;
    } else {
        limit = 255;
        if (inputState == limit) {
            if (card_busy != 0) {
                goto common;
            }
        }
        newState = limit;
    }

common:
    pad_type = (u8)newState;
    if (oldState == newState) {
        goto ret0;
    }
    if (limit == newState) {
        pad_type = (u8)newState;
        if (oldState == limit) {
            goto ret0;
        }
    }

    if (newState == 0) {
        pad_type = (u8)newState;
        InitPAD(&pad1_buf, 8, &pad2_buf, 8);
        InitMouse(&pad1_buf, &pad2_buf);
        SenseMouse(3, 4);
        SetMouse(0, 160, 120);
        RangeMouse(0, 320, 0, 240);
        cursor_cw_x = 0;
        cursor_cw_y = 0;
        mouse_x = 0;
        mouse_y = 0;
        pad_data = 0;
        pad_prev = 0;
        pad_trig = 0;
        StartPAD();
        ChangeClearPAD(0);
        return 1;
    }

    pad_type = (u8)newState;
    if (newState == 1) {
        InitPAD(&pad1_buf, 8, &pad2_buf, 8);
        pad_data = 0;
        pad_prev = 0;
        pad_trig = 0;
        cursor_cw_x = 0;
        cursor_cw_y = 0;
        StartPAD();
        ChangeClearPAD(0);
    }
    return 1;
ret0:
    return 0;
}
