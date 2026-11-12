/* Size: 44 bytes */
/* @SymbolSize: 44 */
#include "types.h"

extern s32 taco_name_col;
extern s32 taco_wheel_no;
extern s32 taco_wheel_ofs;
extern s32 taco_wheel_spd;
extern s32 taco_wheel_slide;

void taco_wheel_clear(void)
{
    taco_name_col = 0;
    taco_wheel_no = 0;
    taco_wheel_ofs = 0;
    taco_wheel_spd = 0;
    taco_wheel_slide = 0;
}
