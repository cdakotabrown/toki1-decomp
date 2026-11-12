/* @SymbolSize: 40 */
#include "types.h"

extern void sub_sub_step_up();
extern void bustup_cd_read();

void bustup_cd_read625a_step(void)
{
    bustup_cd_read(25178);
    sub_sub_step_up();
}
