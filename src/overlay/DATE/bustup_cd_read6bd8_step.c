/* @SymbolSize: 40 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void bustup_cd_read(s32 a0);

void bustup_cd_read6bd8_step(void)
{
    bustup_cd_read(27608);
    sub_sub_step_up();
}
