/* Address: 0x800628C4 */
/* Size: 108 bytes (27 instructions) */
/* Region: Game code (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* Post-processing required:
 *   1. patch_stack_complete.py --old 24 --new 40
 *   2. swap_registers_auto.py --swaps v0:v1
 */
extern void white_light(int);
extern void goto_step(int);
extern void game_reset(void);

extern int work_fuku;

void title_exit(void) {
    white_light(255);
    switch (work_fuku) {
        case 0:
            goto_step(19);
            break;
        case 1:
            goto_step(32);
            break;
        default:
            game_reset();
            break;
    }
}
