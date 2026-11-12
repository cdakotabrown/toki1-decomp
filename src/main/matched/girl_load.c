/* Function: girl_load */
/* Address: 0x80084734 */
/* Size: 300 bytes (75 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x8008472C */
/* @SymbolSize: 308 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 now_girl;
extern u8 now_year;

extern void bustup_cd_read(s32);
extern void sub_sub_step_up(void);

void girl_load(void) {
    switch (now_girl) {
    case 0:
        bustup_cd_read(22118);
        break;
    case 1:
        bustup_cd_read(24143);
        break;
    case 2:
        bustup_cd_read(21308);
        break;
    case 3:
        bustup_cd_read(22973);
        break;
    case 4:
        bustup_cd_read(26573);
        break;
    case 5:
        bustup_cd_read(24773);
        break;
    case 6:
        bustup_cd_read(25403);
        break;
    case 7:
        bustup_cd_read(23558);
        break;
    case 8:
        if (now_year == 95) {
            bustup_cd_read(20723);
        } else if (now_year == 96) {
            bustup_cd_read(20768);
        } else {
            bustup_cd_read(20813);
        }
        break;
    case 9:
        bustup_cd_read(26033);
        break;
    case 10:
        bustup_cd_read(27203);
        break;
    }

    sub_sub_step_up();
}
