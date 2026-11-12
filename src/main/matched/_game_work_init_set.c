/* Address: 0x80041A38 */
/* Size: 56 bytes (14 instructions) */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void game_work_init1(void);
extern void parameter_init(void);
extern void girl_rec_init(void);
extern void work_clear(void);

void _game_work_init_set(void)
{
    game_work_init1();
    parameter_init();
    girl_rec_init();
    work_clear();
}
