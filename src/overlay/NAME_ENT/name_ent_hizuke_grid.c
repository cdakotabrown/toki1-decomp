/* name_ent_hizuke_grid - NAME_ENT overlay, Address: 0x80137B50, Size: 228 bytes (57 insns) */
#include "types.h"

extern void name_ent_digit_disp();
extern void name_ent_num_disp();
extern void goto_tpage();

void name_ent_hizuke_grid(void)
{
    s32 i;
    s32 yPos;

    i = 1;
    yPos = -112;
    while (i < 10) {
        name_ent_digit_disp(i, yPos, 8);
        i++;
        yPos += 24;
    }

    i = 0;
    yPos = 112;
    while (i < 2) {
        name_ent_num_disp(i + 10, yPos, 8);
        i++;
        yPos += 24;
    }

    i = 0;
    yPos = -104;
    while (i < 11) {
        name_ent_num_disp(i + 12, yPos, 24);
        i++;
        yPos += 24;
    }

    i = 0;
    yPos = -104;
    while (i != 9) {
        name_ent_num_disp(i + 23, yPos, 40);
        i++;
        yPos += 24;
    }

    goto_tpage(320, 0, 2, 0, 0);
}
