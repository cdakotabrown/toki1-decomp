/* @SymbolStart: 0x8015662C */
/* @SymbolSize: 464 */
/* @RodataBase: 0x8012008C */
/* Address: 0x80156634 */
/* Size: 464 bytes (116 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B0F30;

extern void func_8010BC54(void);
extern void func_8010C168(void);
extern void func_8010C64C(void);
extern void func_8010CB30(void);
extern void func_8010D014(void);
extern void func_8010D528(void);
extern void func_8010DA3C(void);
extern void func_8010DF50(void);
extern void func_8010E464(void);
extern void func_8010E948(void);
extern void func_8010EE2C(void);
extern void func_80015D28(s32 id, s32 base, s32 off);
extern void func_80011DFC(void);

void func_80156634(void) {
    u8 step_v = D_800B0F30;

    switch ((u32)step_v) {
    case 0:
        func_8010BC54();
        func_80015D28(23, 0x80198000, 0x9ED8);
        break;
    case 1:
        func_8010C168();
        func_80015D28(24, 0x80198000, 0x9F31);
        break;
    case 2:
        func_8010C64C();
        func_80015D28(21, 0x80198000, 0x9EEF);
        break;
    case 3:
        func_8010CB30();
        func_80015D28(22, 0x80198000, 0x9F04);
        break;
    case 4:
        func_8010D014();
        func_80015D28(22, 0x80198000, 0x9F8A);
        break;
    case 5:
        func_8010D528();
        func_80015D28(23, 0x80198000, 0x9F49);
        break;
    case 6:
        func_8010DA3C();
        func_80015D28(21, 0x80198000, 0x9F60);
        break;
    case 7:
        func_8010DF50();
        func_80015D28(23, 0x80198000, 0x9F1A);
        break;
    case 8:
        func_8010E464();
        func_80015D28(19, 0x80198000, 0x9EC5);
        break;
    case 9:
        func_8010E948();
        func_80015D28(21, 0x80198000, 0x9F75);
        break;
    default:
        func_8010EE2C();
        func_80015D28(22, 0x80198000, 0x9FA0);
        break;
    }

    func_80011DFC();
}
