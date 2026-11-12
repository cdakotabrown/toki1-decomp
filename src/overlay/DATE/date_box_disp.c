/* date_box_disp -- DATE overlay, Address: 0x8014E0B0, 120 insns. */
#include "types.h"

extern void date_quad_disp(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5,
                          s32 a6, s32 a7, s32 a8, s32 a9, s32 a10, s32 a11);
extern void dtd_on_tpage(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);

void date_box_disp(void)
{
    s32 white = 0x00FFFF00;

    date_quad_disp(-150, -120, -155, -120, -80, 40, -105, 40, white, white, 9, 1);
    date_quad_disp(-160, -120, -165, -120, -100, 40, -115, 40,
                  0x00FFFFFF, 0x00FFFFFF, 9, 1);
    date_quad_disp(-162, -120, -167, -120, -110, 40, -135, 40, white, white, 9, 1);
    dtd_on_tpage(0, 0, 9, 3, 0);
    date_quad_disp(-125, -120, -130, -120, -40, 40, -50, 40, white, white, 13, 1);
    date_quad_disp(-132, -120, -137, -120, -60, 40, -70, 40,
                  0x00FFFFFF, 0x00FFFFFF, 13, 1);
    dtd_on_tpage(0, 0, 13, 3, 0);
}
