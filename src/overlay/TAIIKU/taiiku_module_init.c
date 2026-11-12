/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_registers v1:a0 --bidirectional */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x88 */
/* @PostProc: patch_insn_reg_field 0x2c:rd:v1:a0 */
/* @PostProc: patch_insn_reg_field 0x38:rd:v1:a0 */
/* @PostProc: patch_insn_reg_field 0x44:rd:v1:a0 */
/* Address: 0x801460B8 */
/* Size: 152 bytes (38 insns) */
#include "types.h"

extern s32 taiiku_spoon_buf;
extern s32 taiiku_spoon_buf_2;
extern s32 taiiku_spoon_buf_3;
extern s32 screen_w[];

extern s32 D_800EE760_b[];

extern void srn_vram_set();
extern void srn_init();
extern void mod_init();

void taiiku_module_init(void) {
    s32 *v0;
    s32 *a0;
    s32 v1;

    mod_init(0, taiiku_spoon_buf_2, taiiku_spoon_buf);
    srn_init(0, taiiku_spoon_buf_3, 0);
    srn_vram_set(0, 5, 1408, 240);

    v0 = (s32 *)&screen_w[0];
    a0 = (s32 *)&screen_w[3];
    v1 = 8;
    do {
        v0++;
        *(s32 *)((char *)v0 + 4664) = v1;
    } while (v0 < a0);

    v0 = (s32 *)&D_800EE760_b[0];
    a0 = (s32 *)&D_800EE760_b[3];
    do {
        v0++;
        *(s32 *)((char *)v0 + 4792) = v1;
    } while (v0 != a0);
}
