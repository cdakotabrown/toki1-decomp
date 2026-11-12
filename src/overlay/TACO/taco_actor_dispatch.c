/* @SymbolStart: 0x8015A958 */
/* @SymbolSize: 464 */
/* @RodataBase: 0x8015DF70 */
/* Address: 0x8015A958 (cross-prologue head) / 0x8015A95C (prologue) */
/* Size: 464 bytes (116 insns incl. the 1-insn head) */
#include "types.h"

extern void taco_actor_step1(s32);
extern void taco_actor_step2(s32);
extern void taco_actor_step3(s32);
extern void taco_actor_step4(s32);
extern void taco_actor_step5(s32);
extern void taco_actor_step6(s32);
extern void taco_actor_step7(s32);
extern void func_801570B4(s32);
extern void taco_actor_step14(s32);
extern void taco_actor_step13(s32);
extern void taco_actor_step12(s32);
extern void taco_actor_step11(s32);
extern void taco_actor_step15(s32);
extern void func_80158F58(s32);
extern void taco_actor_homing(s32);
extern void taco_actor_shock(s32);
extern void taco_actor_ramp(s32);
extern void taco_actor_step23(s32);
extern void taco_actor_rise(s32);
extern void taco_actor_fall(s32);
extern void taco_actor_stepff(s32);

void taco_actor_dispatch(u8 sel, s32 arg)
{
    switch ((u32)sel) {
    case 1:
        taco_actor_step1(arg);
        break;
    case 2:
        taco_actor_step2(arg);
        break;
    case 3:
        taco_actor_step3(arg);
        break;
    case 4:
        taco_actor_step4(arg);
        break;
    case 5:
        taco_actor_step5(arg);
        break;
    case 6:
        taco_actor_step6(arg);
        break;
    case 7:
        taco_actor_step7(arg);
        break;
    case 16:
        func_801570B4(arg);
        break;
    case 19:
        taco_actor_step13(arg);
        break;
    case 20:
        taco_actor_step14(arg);
        break;
    case 18:
        taco_actor_step12(arg);
        break;
    case 17:
        taco_actor_step11(arg);
        break;
    case 21:
        taco_actor_step15(arg);
        break;
    case 22:
        func_80158F58(arg);
        break;
    case 32:
        taco_actor_homing(arg);
        break;
    case 33:
        taco_actor_shock(arg);
        break;
    case 34:
        taco_actor_ramp(arg);
        break;
    case 35:
        taco_actor_step23(arg);
        break;
    case 253:
        taco_actor_fall(arg);
        break;
    case 254:
        taco_actor_rise(arg);
        break;
    case 255:
        taco_actor_stepff(arg);
        break;
    }
}
