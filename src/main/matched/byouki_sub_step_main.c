/* Function: byouki_sub_step_main */
/* Address: 0x80072D60 */
/* Size: 208 bytes (52 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

#ifdef PC_PORT

extern s32 step_counter;
extern u8 now_sub_sub_step;
#define STEP_FRAME step_counter
#define STEP_SUB now_sub_sub_step
#else
typedef struct {
    s32 frame;
    u8 pad4[2];
    u8 sub_step;
} StepState;
extern StepState step_counter;
#define STEP_FRAME step_counter.frame
#define STEP_SUB step_counter.sub_step
#endif
extern void byouki_hassei(void);
extern void byouki_kega(void);
extern void byouki_noiroze(void);
extern void byouki_sick(void);
extern void byouki_kega_kaifuku(void);
extern void byouki_noiroze_kaifuku(void);
extern void byouki_main7(void);
extern void goto_sub_step(s32);

void byouki_sub_step_main(void) {
    STEP_FRAME++;
    switch (STEP_SUB) {
    case 0:
        byouki_hassei();
        break;
    case 1:
        byouki_kega();
        break;
    case 2:
        byouki_noiroze();
        break;
    case 3:
        byouki_sick();
        break;
    case 5:
        byouki_kega_kaifuku();
        break;
    case 6:
        byouki_noiroze_kaifuku();
        break;
    case 7:
        byouki_main7();
        break;
    case 4:
    default:
        goto_sub_step(255);
        break;
    }
}
