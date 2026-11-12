/* kangei_choice_menu_step - KANGEI overlay, Address: 0x80136910, Size: 48 bytes (12 insns) */
/* @SymbolSize: 48 */
#include "types.h"

extern s32 kangei_choice_timer;
extern void menu2_main(void);

void kangei_choice_menu_step(void)
{
    s32 hidden_t6 = kangei_choice_timer;

    kangei_choice_timer = hidden_t6 + 1;
    menu2_main();
}
