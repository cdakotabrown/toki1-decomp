/* @RodataBase: 0x80145DB0 */
/* @SymbolStart: 0x80141160 */
/* @SymbolSize: 436 */
/* func_80141168 -- GYOZI overlay, Address: 0x80141168, Size: 428 bytes (107 insns) */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:28 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t4:t5 0x48:rt:t3:t4 0x60:rs:t4:t5 0x60:rt:t4:t5 0x68:rs:t4:t5 0x6c:rd:t5:t6 0x70:rd:t6:t8 0x70:rt:t5:t6 0x74:rs:t6:t8 0x74:rt:t3:t4 0x78:rt:t8:t9 0x7c:rs:t8:t9 0x7c:rt:t8:t9 0x84:rs:t8:t9 0x8c:rd:t8:t9 0x8c:rt:t8:t9 0x94:rt:t8:t9 0x98:rt:t8:t9 0xa0:rs:t8:t9 0x150:rt:t9:t2 0x154:rs:t9:t2 0x154:rt:t9:t2 0x160:rt:t9:t2 */
#include "types.h"

extern u8 D_800F647A;

extern void func_80052060(void);
extern void func_80143314(void);
extern void func_801433E0(void);
extern void func_80143520(void);
extern void func_801435C4(void);
extern void func_801436E4(void);
extern void func_80143780(void);
extern void func_80143858(void);
extern void func_80143940(void);
extern void func_801439F0(void);

typedef union {
    s32 w;
    struct {
        u16 lo;
        s16 hi;
    } h;
} RecHead;

typedef struct {
    RecHead head;
    u8 pad[52];
} Rec56;

typedef struct {
    u8 head[432];
    Rec56 recs[11];
} Blob53A0;

extern Blob53A0 D_800F53A0;
extern RecHead D_80148970;

extern volatile u8 D_800F62CF;
extern volatile u8 D_8012B8C7;
extern s16 D_8012B8C8;
extern volatile s16 D_8012B8D6;
extern volatile s16 D_8012B8D8;
extern volatile u8 D_801317EB;

s32 func_80141168(void)
{
    s32 t6 = D_800F647A;
    s32 counter;

    if (t6 == 0) {
        D_80148970 = D_800F53A0.recs[D_800F62CF].head;
        D_800F53A0.recs[D_800F62CF].head.h.hi = 50;
    }

    switch (D_800F62CF) {
    case 0:
        func_80052060();
        break;
    case 1:
        func_80143314();
        break;
    case 2:
        func_801433E0();
        break;
    case 3:
        func_80143520();
        break;
    case 4:
        func_801435C4();
        break;
    case 5:
        func_801436E4();
        break;
    case 6:
        func_80143780();
        break;
    case 7:
        func_80143858();
        break;
    case 8:
        func_80143940();
        break;
    case 9:
        func_801439F0();
        break;
    case 10:
        func_80052060();
        break;
    }

    D_8012B8C7 = D_801317EB;

    if (D_8012B8D8 != 0) {
        return;
    }
    if (D_8012B8C8 != 0) {
        return;
    }
    counter = (s16)(D_8012B8D6 + 1);
    D_8012B8D6 = counter;
    if (counter >= 3) {
        D_8012B8D6 = 0;
        counter = 0;
    }
    return counter;
}
