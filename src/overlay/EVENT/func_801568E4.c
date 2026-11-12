/* @PostProc: patch_stack_complete --old 184 --new 192 */
/* @PostProc: remap_stack_slots --map 52:56,96:100,140:144 */
/* Address: 0x801568E4 */
/* Size: 540 bytes (135 instructions) */
#include "types.h"

typedef struct {
    u16 v[21];
} EventTbl;

extern volatile u8 D_801243B0;
extern EventTbl D_80125454;
extern EventTbl D_80125480;
extern EventTbl D_801254AC;

extern void func_800143DC();
extern void func_80011DFC();

void func_801568E4(void) {
    EventTbl a;
    EventTbl b;
    EventTbl c;
    s32 step;
    s32 msg1;
    s32 msg2;
    s32 kind;

    step = D_801243B0;
    a = D_80125454;
    b = D_80125480;
    c = D_801254AC;

    msg1 = 0xAF42;
    msg2 = 0xAF23;
    kind = 1;

    if (step == 2) {
        msg1 = 0xB384;
        msg2 = 0xB381;
    } else if (step == 17) {
        msg1 = 0xB3DA;
        msg2 = 0xB3CB;
    } else if (step == 1 || step == 3) {
        msg1 = 0xB163;
        msg2 = 0xB14D;
    } else if (step == 10 || step == 9) {
        msg1 = 0xB172;
        msg2 = 0xB165;
    } else if (step == 12) {
        msg1 = 0xB444;
        msg2 = 0xB42F;
    } else if (step == 17 || step == 16) {
        msg1 = 0xB3DA;
        msg2 = 0xB3CB;
    } else if (step == 20 || step == 19) {
        msg1 = 0xB41D;
        msg2 = 0xB410;
    } else if (step == 8) {
        msg1 = 0xB054;
        msg2 = 0xB04A;
    } else if (step == 7) {
        msg1 = 0xB304;
        msg2 = 0xB2EB;
    }

    func_800143DC(kind, msg1, msg2, b.v[step], c.v[step], a.v[step]);
    func_80011DFC();
}
