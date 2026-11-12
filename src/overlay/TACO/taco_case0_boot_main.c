/* @SymbolStart: 0x80133870 */
/* @SymbolSize: 272 */
/* @PostProc: swap_instructions --offset1 0xfc --offset2 0x100 */
/* @PostProc: swap_instructions --offset1 0xf8 --offset2 0xfc */
/* @PostProc: swap_instructions --offset1 0xf4 --offset2 0xf8 */
/* @PostProc: swap_instructions --offset1 0xf0 --offset2 0xf4 */
/* @PostProc: swap_instructions --offset1 0xec --offset2 0xf0 */
/* @PostProc: swap_instructions --offset1 0xe8 --offset2 0xec */
/* @PostProc: swap_instructions --offset1 0xe4 --offset2 0xe8 */
/* @PostProc: swap_instructions --offset1 0xe0 --offset2 0xe4 */
/* @RodataBase: 0x8015D550 */
/* @SymbolSize: 244 */
#include "types.h"

extern void taco_boot_init_load(void);
extern void device_pair_load(void);
extern void taco_tile_page_load(void);
extern void taco_zk_cd_load(void);
extern void taco_view_init(void);
extern void taco_str_disp2(void *a0, s32 a1, s32 a2, s32 a3);
extern void taco_num_disp(s32 a0, s32 a1, s32 a2, s32 a3);

typedef struct {
    u32 frame;
    u8  pad4;
    u8  step1;
    u8  step;
    u8  pad7;
} TacoStep;
extern TacoStep step_counter;
extern s16 taco_time_left;
extern u8 taco_main0_str0[];

void taco_case0_boot_main(void) {
    u32 flags;

    step_counter.frame += 1;
    switch (step_counter.step) {
    case 0:
        taco_boot_init_load();
        break;
    case 1:
        device_pair_load();
        break;
    case 2:
        taco_tile_page_load();
        break;
    case 3:
        taco_zk_cd_load();
        break;
    case 4:
        taco_view_init();
        break;
    }

    if (step_counter.frame & 0x10) {
        taco_str_disp2(taco_main0_str0, -96, 0, 1);
    }

    flags = step_counter.frame;
    if (flags != 0 && flags < 1024) {
        taco_num_disp(taco_time_left--, 0, 50, 6);
    }
}
