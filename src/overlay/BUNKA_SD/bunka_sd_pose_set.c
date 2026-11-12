/* @SymbolSize: 1516 */
/* @RodataBase: 0x8013B580 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=190:t1,159:a1,51:t0,81:a2,133:a3,134:a1 */
/* @PostProc: swap_instructions --offset1 0x28c --offset2 0x294 */
/* @PostProc: swap_instructions --offset1 0x290 --offset2 0x294 */
/* @PostProc: swap_instructions --offset1 0x2a8 --offset2 0x2ac */
/* @PostProc: swap_instructions --offset1 0x2bc --offset2 0x2c0 */
/* @PostProc: swap_instructions --offset1 0x458 --offset2 0x460 */
/* @PostProc: swap_instructions --offset1 0x4c8 --offset2 0x4d0 */
/* @PostProc: swap_instructions --offset1 0x540 --offset2 0x548 */
/* @PostProc: swap_instructions --offset1 0x5bc --offset2 0x5c4 */
/* @PostProc: swap_branch_operands --offsets 0x234,0x24c,0x2d8,0x490,0x500,0x508,0x584 */
/* bunka_sd_pose_set - BUNKA_SD overlay, Address: 0x80139200 */
/* Size: 1516 bytes (379 insns), leaf, no stack frame. */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 step_counter;
extern u8  now_year;
extern s32 bunka_sd_kind;
extern s16 cw97_frame;
extern s16 cw98_frame;
extern CLASS_W class_w[];

void bunka_sd_pose_set(void)
{
    u32 step;
    u32 sub;
    u32 idx;
    u32 charId;
    s32 flags;

    step = (u32)step_counter;

    switch (step) {
    case 29:
        cw97_frame = 1;
        break;
    case 88:
        cw97_frame = 0;
        break;
    case 102:
        cw97_frame = 2;
        break;
    case 131:
        cw97_frame = 3;
        break;
    }

    if (step >= 191 && step < 0x70000000) {
        idx = ((step - 190) % 242) / 11;
        switch (idx) {
        case 0:
            cw97_frame = 4;
            break;
        case 1:
            cw97_frame = 3;
            break;
        case 2:
            cw97_frame = 5;
            break;
        case 3:
            cw97_frame = 6;
            break;
        case 4:
            cw97_frame = 3;
            break;
        case 5:
            cw97_frame = 12;
            break;
        case 6:
            cw97_frame = 11;
            break;
        case 7:
            cw97_frame = 14;
            break;
        case 8:
            cw97_frame = 3;
            break;
        case 9:
            cw97_frame = 8;
            break;
        case 10:
            cw97_frame = 7;
            break;
        case 11:
            cw97_frame = 9;
            break;
        case 12:
            cw97_frame = 3;
            break;
        case 13:
            cw97_frame = 5;
            break;
        case 14:
            cw97_frame = 6;
            break;
        case 15:
            cw97_frame = 3;
            break;
        case 16:
            cw97_frame = 13;
            break;
        case 17:
            cw97_frame = 11;
            break;
        case 18:
            cw97_frame = 4;
            break;
        case 19:
            cw97_frame = 3;
            break;
        case 20:
            cw97_frame = 10;
            break;
        case 21:
            cw97_frame = 7;
            break;
        }
    }

    if (step == 0x70000000) {
        cw97_frame = 3;
    }

    sub = step - 0x70000000;

    charId = now_year;

    switch ((s32)sub) {
    case 59:
        cw97_frame = 2;
        break;
    case 73:
        cw97_frame = 0;
        break;
    case 102:
        cw97_frame = 1;
        break;
    case 161:
        if (charId == 96) {
            cw97_frame = 0;
        }
        break;
    }

    if (step == 149) {
        cw98_frame = 16;
        class_w[99].frame = 20;
        class_w[100].frame = 24;
        class_w[101].frame = 29;
        flags = bunka_sd_kind;
        if (flags & 1) {
            class_w[99].frame = 32;
        }
        if (flags & 2) {
            class_w[100].frame = 36;
        }
        if (flags & 4) {
            class_w[101].frame = 40;
        }
    }

    flags = bunka_sd_kind;

    if (step >= 209 && step < 0x70000000) {
        idx = ((step - 201) % 78) / 39;
        switch (idx) {
        case 0:
            cw98_frame = 17;
            class_w[99].frame = 21;
            class_w[100].frame = 25;
            class_w[101].frame = 29;
            if (flags & 1) {
                class_w[99].frame = 33;
            }
            if (flags & 2) {
                class_w[100].frame = 37;
            }
            if (flags & 4) {
                class_w[101].frame = 41;
            }
            break;
        case 1:
            cw98_frame = 16;
            class_w[99].frame = 20;
            class_w[100].frame = 24;
            class_w[101].frame = 28;
            if (flags & 1) {
                class_w[99].frame = 32;
            }
            if (flags & 2) {
                class_w[100].frame = 36;
            }
            if (flags & 4) {
                class_w[101].frame = 40;
            }
            break;
        }
    }

    if (step == 0x70000000) {
        cw98_frame = 16;
        class_w[99].frame = 20;
        class_w[100].frame = 24;
        class_w[101].frame = 28;
        if (flags & 1) {
            class_w[99].frame = 32;
        }
        if (flags & 2) {
            class_w[100].frame = 36;
        }
        if (flags & 4) {
            class_w[101].frame = 40;
        }
    }

    if ((s32)sub == 59 && charId != 96) {
        cw98_frame = 15;
        class_w[99].frame = 19;
        class_w[100].frame = 23;
        class_w[101].frame = 27;
        if (flags & 1) {
            class_w[99].frame = 31;
        }
        if (flags & 2) {
            class_w[100].frame = 35;
        }
        if (flags & 4) {
            class_w[101].frame = 39;
        }
    }

    if ((s32)sub == 104 && charId == 96) {
        cw98_frame = 18;
        class_w[99].frame = 22;
        class_w[100].frame = 26;
        class_w[101].frame = 30;
        if (flags & 1) {
            class_w[99].frame = 34;
        }
        if (flags & 2) {
            class_w[100].frame = 38;
        }
        if (flags & 4) {
            class_w[101].frame = 42;
        }
    }
}
