/* rpg_bat_entry - RPG_BAT overlay, Address: 0x80137204, Size: 116 bytes (29 insns) */
#include "types.h"

typedef struct {
    u8 pad0[2];
    u8 flags;
    u8 pad1[39];
    s16 counter;
    u8 pad2[24];
} BatEntry;

extern BatEntry class_w[];

extern s32 rpg_bat_anim_set();

void rpg_bat_entry(void)
{
    BatEntry *ptr;
    s32 i;

    ptr = class_w;
    for (i = 0; i < 12; i++) {
        ptr[100].counter++;
        if ((ptr[100].flags & 1) == 0) {
            rpg_bat_anim_set(i + 4, 255, 1, 0);
        }
        ptr++;
    }
}
