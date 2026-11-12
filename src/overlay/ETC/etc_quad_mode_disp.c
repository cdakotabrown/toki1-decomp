/* @CompileFlags: -cckr -Wc,-pic0 */
/* @PostProc: patch_stack_complete --old 160 --new 144 */
/* @PostProc: remap_stack_slots --keep-frame --map 120:80,124:84,152:132 */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x3C --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x260 --offset2 0x264 */
/* @PostProc: swap_instructions --offset1 0x85C --offset2 0x860 */
/* @PostProc: swap_instructions --offset1 0x8FC --offset2 0x904 */
/* @PostProc: patch_insn_reg_field 0x19c:rd:a0:v1 0x1a0:rs:a0:v1 0x1a0:rt:a0:v1 0x1a8:rt:a0:v1 0x208:rs:a0:v1 0x23c:rt:a0:v1 0x244:rd:v1:a0 0x25c:rt:v1:a0 0x29c:rt:v1:a0 0x35c:rd:a0:v1 0x360:rs:a0:v1 0x360:rt:a2:a1 0x368:rt:a2:a1 0x3c8:rd:a1:a2 0x3c8:rs:a0:v1 0x3cc:rs:a1:a2 0x3cc:rt:a1:a2 0x3d0:rt:a1:a2 0x3fc:rt:a2:a1 0x404:rd:v1:a0 0x418:rt:v1:a0 0x42c:rt:a1:a2 0x448:rt:v1:a0 0x50c:rd:a0:v1 0x510:rs:a0:v1 0x510:rt:a0:v1 0x518:rt:a0:v1 0x5a8:rs:a0:v1 0x5e0:rt:a0:v1 0x5f8:rd:v1:a0 0x608:rt:v1:a0 0x648:rt:v1:a0 0x70c:rd:a0:v1 0x710:rs:a0:v1 0x710:rt:a2:a1 0x718:rt:a2:a1 0x7a8:rd:a1:a2 0x7a8:rs:a0:v1 0x7ac:rs:a1:a2 0x7ac:rt:a1:a2 0x7b0:rt:a1:a2 0x7e0:rt:a2:a1 0x7e8:rd:v1:a0 0x800:rt:v1:a0 0x814:rt:a1:a2 0x834:rt:v1:a0 */
#include "types.h"

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    u16 clut;
    s16 x1, y1;
    u8 u1, v1;
    u16 tpage;
    s16 x2, y2;
    u8 u2, v2;
    u16 pad1;
    s16 x3, y3;
    u8 u3, v3;
    u16 pad2;
} PolyFT4;

extern PolyFT4 *prim_alloc();
extern s32 GetTPage();
extern void AddPrim();
extern void SetSemiTrans();
extern void SetShadeTex();
extern void SetPolyFT4();

extern s16 screen_w[];
extern u8  ot_buff0[];
extern s32 activeBuff;

#define YB (screen_w[activeBuff + 12])

void etc_quad_mode_disp(P, mode)
s32 P;
s32 mode;
{
    PolyFT4 *arr[2];
    s32 i;
    s32 a = 80;
    s32 b = 68;
    s32 c = 136;
    s32 tx;
    u8 hh = 160;
    s32 uv[2];
    s32 tu, tv;
    s32 x, h, y;

    uv[1] = 24; uv[0] = 160;

    for (i = 0, tx = 320; i < 2; i++) {
        arr[i] = prim_alloc(40, activeBuff);
        SetPolyFT4(arr[i]);
        SetSemiTrans(arr[i], 0);
        SetShadeTex(arr[i], 1);
        arr[i]->tpage = GetTPage(2, 0, tx, 256);

        switch (mode) {
        case 0:
            x = 24 - 11 * P / a + i * c - P * i * b / a;
            arr[i]->x0 = x;
            arr[i]->y0 = YB - 101;
            arr[i]->x1 = x - P * b / a + 136;
            arr[i]->y1 = YB - 101;
            arr[i]->x2 = x;
            h = hh - P;
            arr[i]->y2 = YB + h - 101;
            arr[i]->x3 = 160 - 11 * P / a + i * c - P * i * b / a - P * b / a;
            arr[i]->y3 = YB + h - 101;
            break;
        case 1:
            x = 141 * P / a + i * c - P * i * b / a;
            arr[i]->x0 = x + 24;
            arr[i]->y0 = YB - 101;
            arr[i]->x1 = x - P * b / a + 160;
            arr[i]->y1 = YB - 101;
            arr[i]->x2 = x + 24;
            h = hh - P;
            arr[i]->y2 = YB + h - 101;
            arr[i]->x3 = 160 + 141 * P / a + i * c - P * i * b / a - P * b / a;
            arr[i]->y3 = YB + h - 101;
            break;
        case 2:
            x = 24 - 11 * P / a + i * c - P * i * b / a;
            arr[i]->x0 = x;
            y = 112 * P / a;
            arr[i]->y0 = YB + y - 101;
            arr[i]->x1 = x - P * b / a + 136;
            arr[i]->y1 = YB + y - 101;
            arr[i]->x2 = x;
            h = hh - P;
            arr[i]->y2 = YB + y + h - 101;
            arr[i]->x3 = 160 - 11 * P / a + i * c - P * i * b / a - P * b / a;
            arr[i]->y3 = YB + y + h - 101;
            break;
        case 3:
            x = 141 * P / a + i * c - P * i * b / a;
            arr[i]->x0 = x + 24;
            y = 112 * P / a;
            arr[i]->y0 = YB + y - 101;
            arr[i]->x1 = x - P * b / a + 160;
            arr[i]->y1 = YB + y - 101;
            arr[i]->x2 = x + 24;
            h = hh - P;
            arr[i]->y2 = YB + y + h - 101;
            arr[i]->x3 = 160 + 141 * P / a + i * c - P * i * b / a - P * b / a;
            arr[i]->y3 = YB + y + h - 101;
            break;
        }

        tv = uv[1];
        arr[i]->u0 = tv;
        arr[i]->v0 = 19;
        tu = uv[0];
        arr[i]->u1 = tu;
        arr[i]->v1 = 19;
        arr[i]->u2 = tv;
        arr[i]->v2 = 179;
        arr[i]->u3 = tu;
        arr[i]->v3 = 179;
        arr[i]->r0 = 128;
        arr[i]->g0 = 128;
        arr[i]->b0 = 128;
        AddPrim(&ot_buff0[activeBuff * 1024 + 40], arr[i]);

        uv[1] += 8;
        uv[0] += 8;
        tx += 128;
    }
}
