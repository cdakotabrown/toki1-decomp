/* @PostProc: patch_insn_reg_field 0x58:rt:a2:a0 0x60:rt:a2:a0 0x90:rs:a2:a0 0xac:rt:a2:a0 */
/* @SymbolSize: 392 */
/* Address: 0x8013E514 */
/* Size: 392 bytes (98 instructions) */
#include "types.h"

extern s32 prim_alloc();
extern void safe_env();
extern void SetDrawMode();
extern s32 GetTPage();
extern void AddPrim();
extern void SetSemiTrans();
extern void SetShadeTex();
extern void SetPolyF4();
extern volatile s32 activeBuff;
extern s16 screen_w[];
extern u8 ot_buff0[];

typedef struct {
    s32 unk0;
    s8 unk4;
    s8 unk5;
    s8 unk6;
    s8 pad7;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
} Obj;

void etc_hizuke_bar_disp(s32 a0, s32 a1)
{
    Obj *o;
    void *r;

    o = (Obj *)prim_alloc(24, activeBuff);
    SetPolyF4(o);
    SetSemiTrans(o, 1);
    SetShadeTex(o, 0);
    o->unk8 = (s16)a0;
    o->unkA = screen_w[activeBuff + 12] + 43;
    o->unkC = (s16)(a0 + a1);
    o->unkE = screen_w[activeBuff + 12] + 43;
    o->unk10 = (s16)a0;
    o->unk12 = screen_w[activeBuff + 12] + 55;
    o->unk14 = (s16)(a0 + a1);
    o->unk16 = screen_w[activeBuff + 12] + 55;
    o->unk6 = 0;
    o->unk5 = 0;
    o->unk4 = 0;
    AddPrim(&ot_buff0[activeBuff * 1024 + 12], o);

    o = (Obj *)prim_alloc(12, activeBuff);
    r = (void *)GetTPage(1, 0, 0, 0);
    SetDrawMode(o, 0, 0, r, 0);
    AddPrim(&ot_buff0[activeBuff * 1024 + 12], o);
    safe_env(3);
}
