/* @SymbolStart: 0x80132658 */
/* @SymbolSize: 340 */
/* func_80132660 -- GYOZI overlay, Address: 0x80132660, Size: 336 bytes (84 insns) */
/* @PostProc: swap_instructions --offset1 0xfc --offset2 0x100 */
/* @PostProc: swap_instructions --offset1 0x100 --offset2 0x104 */
/* @PostProc: patch_insn_reg_field 0x110:rd:t7:t6 0x118:rd:t8:t7 0x11c:rt:t8:t7 0x140:rt:t6:t5 0x148:rs:t7:t6 0x148:rt:t6:t5 */
#include "types.h"

extern void func_8004EDE0(s32 a0, s32 a1);
extern void func_80054864(s32 a0);
extern void func_8004C670(void);
extern void func_8005493C(s32 a0);
extern void func_8004EDF4(s32 a0);
extern void func_80053EFC(void);
extern void func_8005ABC0(void);
extern void func_800BCE10(void *a0, void *a1);
extern void func_8009068C(void);
extern void func_80089200(void);
extern void func_800744A0(void);
extern void func_80074950(void);
extern void func_8008FC10(void);
extern void func_80134000(void);
extern void func_8008F618(s32 a0);
extern void func_8004DE1C(void);

typedef struct {
    s32 head;
    u8 pad4[4];
    s32 f8;
    u8 pad12[44];
} Rec56b;

typedef struct {
    u8 head[432];
    Rec56b recs[11];
} Blob53A0b;

extern Blob53A0b D_800F53A0;
extern u8 D_800D9388[];
extern char D_800D92A0[];
extern char D_80145860[];

extern u8 D_800F5AAE;
extern u8 D_800F6412;
extern volatile s32 D_800F6458;
extern u8 D_800F53DA;
extern u8 D_801317EB;
extern u8 D_800C51C4;
extern s16 D_80145EB4;
extern s16 D_80145EB8;
extern u8 D_80145EC8;
extern void func_8004DE1C(void);

void func_80132660(void)
{
    s16 i;
    u32 t6 = D_800F5AAE;

    D_800F5AAE = t6 | 8;
    func_8004EDE0(1, 0);
    func_80054864(1);
    func_8004C670();
    func_8005493C(0);
    func_8004EDF4(1);
    func_80053EFC();
    func_8005ABC0();
    func_800BCE10(D_800D92A0, D_80145860);
    D_800F6412 = 0;
    D_800F6458 = 1;
    func_8009068C();
    D_800F53DA = 128;
    D_801317EB = 0;
    D_800C51C4 = 0;
    func_80089200();
    func_800744A0();
    func_80074950();
    func_8008FC10();
    func_80134000();
    func_8008F618(0);
    D_80145EB4 = 0;
    D_80145EB8 = 0;
    D_80145EC8 = 0;

    i = 0;
    do {
        D_800D9388[i] = ((((u32)(D_800F53A0.recs[i].f8 << 30) >> 31) == 0) ^ 1);
        i = i + 1;
    } while (i < 11);

    func_8004DE1C();
}
