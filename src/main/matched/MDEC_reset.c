/* Function: MDEC_reset */
/* Address: 0x80086B48 */
/* Size: 244 bytes (61 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 *libpress_mdec_ctrl_reg_ptr;
extern s32 *libpress_dma0_chcr_reg_ptr;
extern volatile s32 *libpress_dma1_chcr_reg_ptr;
extern s32 mdec_iq;
extern s32 mdec_coef;

extern void MDEC_in(s32 *, s32);
extern void printf(char *, s32);
extern void exit(void);

extern char libpress_mdec_reset_badopt_fmt[];

void MDEC_reset(s32 arg0) {
    switch (arg0) {
        case 0:
            *libpress_mdec_ctrl_reg_ptr = (s32)0x80000000;
            *libpress_dma0_chcr_reg_ptr = 0;
            *libpress_dma1_chcr_reg_ptr = 0;
            *libpress_mdec_ctrl_reg_ptr = (s32)0x60000000;
            MDEC_in(&mdec_iq, 32);
            MDEC_in(&mdec_coef, 32);
            break;
        case 1:
            *libpress_mdec_ctrl_reg_ptr = (s32)0x80000000;
            *libpress_dma0_chcr_reg_ptr = 0;
            *libpress_dma1_chcr_reg_ptr = 0;
            (void)*libpress_dma1_chcr_reg_ptr;
            *libpress_mdec_ctrl_reg_ptr = (s32)0x60000000;
            break;
        default:
            printf(libpress_mdec_reset_badopt_fmt, arg0);
            exit();
            break;
    }
}
