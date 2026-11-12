/* Function: icon_cursor_move */
/* Address: 0x80065C74 */
/* Size: 1008 bytes (252 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_80065C74.py */
#include "types.h"

extern s32 pad_data;
extern s8 D_800EF8F3;
extern u8 icon_mode;
extern u8 csr_speed;
extern u8 icon_num;
extern s8 D_800BC05C;
extern u8 D_800BC060;
extern s16 D_80127196;
extern s16 D_8012719A;
extern s16 icon_xy[];
extern u8 D_8012DCA0[];
extern u8 menu0_hit1[];
extern u8 D_800EE785;
extern s16 D_80128A9A;
extern u8 class_w[];

extern s32 icon_atari_check(s32, s32, s32);
extern void icon_menu_set(s32);
extern void menu_check(s32, s32, s32);
extern void func_80050030(s32);
extern s32 rsin(s32);

void icon_cursor_move(s32 arg0) {
    s32 padFlags;
    s32 charIdx;
    s32 offsetX;
    s32 offsetY;
    s32 three;
    u8 savedCharIdx;
    s32 loopCount;

    (void)&savedCharIdx;
    padFlags = pad_data;
    charIdx = D_800EF8F3;
    offsetX = 0;
    offsetY = 0;
    savedCharIdx = (u8)charIdx;
    three = 3;

    if (padFlags & 1) {
        if ((u32)icon_mode % (u32)three != 2u) {
            goto after_clamp;
        }
        {
            s32 v0;
            s32 moveSpeed;

            v0 = padFlags & 0xa000;
            if (v0 == 0x2000) {
                moveSpeed = (s32)csr_speed;
                offsetX = moveSpeed;
            } else if (v0 == (s32)0x8000) {
                moveSpeed = (s32)csr_speed;
                offsetX = -moveSpeed;
            } else {
                moveSpeed = (s32)csr_speed;
            }

            v0 = padFlags & 0x5000;
            if (v0 == 0x1000) {
                offsetY = -moveSpeed;
            } else if (v0 == 0x4000) {
                offsetY = moveSpeed;
            }
        }

        if (icon_atari_check((s32)charIdx, offsetX, offsetY) == 0) {
            offsetX = 0;
            offsetY = 0;
        }

        charIdx = D_800EF8F3;
        {
            s16 *pos;
            pos = &icon_xy[charIdx * 2];
            pos[0] += (s16)offsetX;
            pos[1] += (s16)offsetY;
        }
        D_80127196 += (s16)offsetX;
        D_8012719A += (s16)offsetY;
        icon_menu_set((s32)icon_num);

        charIdx = D_800EF8F3;
        {
            s16 *posBase;
            s16 *pos;
            s16 px, py;

            posBase = icon_xy;
            pos = &posBase[charIdx * 2];
            px = pos[0];
            if (px >= 129) {
                pos[0] = 128;
            }
            px = pos[0];
            if (px < -138) {
                pos[0] = -138;
            }
            py = pos[1];
            if (py < -92) {
                pos[1] = -92;
            }
            py = pos[1];
            if (py >= 93) {
                pos[1] = 92;
            }
        }
    } else {
        menu_check(0, D_80127196, D_8012719A);
        func_80050030(0);
        charIdx = D_800EF8F3;
    }

after_clamp:
    {
        s8 savedChar;
        u8 curCount;

        savedChar = D_800BC05C;
        if (savedCharIdx != (u8)savedChar) {
            D_800BC05C = charIdx;
            D_800BC060 = 32;
        } else {
            curCount = D_800BC060;
            if (curCount != 0) {
                D_800BC060 = curCount - 1;
            }
        }
    }

    loopCount = 14;
    if (icon_num == 15) {
        loopCount = 15;
    }
    if (icon_mode == 2) {
        loopCount = 15;
    }

    if (loopCount > 0) {
        s32 i;
        u8 *entry;
        u8 *flagPtr;
        s32 constVal;

        i = 0;
        entry = class_w;
        flagPtr = D_8012DCA0;
        constVal = 32;
        do {
            if (*flagPtr == 0) {
                entry[5442] = 0;
                entry[4419] = 81;
                entry[4353] = three;
            } else {
                entry[5442] = 1;
                entry[4353] = 4;
                entry[5441] = three;
                if (menu0_hit1[i << 4] == 0) {
                    entry[4419] = 83;
                    if (arg0 == 0) {
                        *(s16 *)(entry + 5466) = 4096;
                        *(s16 *)(entry + 5468) = 4096;
                        *(s16 *)(entry + 4378) = 4096;
                        *(s16 *)(entry + 4380) = 4096;
                    } else {
                        *(s16 *)(entry + 5466) = 4096;
                        *(s16 *)(entry + 5468) = 4096;
                    }
                } else {
                    entry[4419] = 82;
                    entry[5441] = 2;
                    entry[4353] = three;
                    if (arg0 == 0) {
                        *(s16 *)(entry + 5466) = (s16)(rsin((constVal - (s32)D_800BC060) << 6) + 4096);
                        *(s16 *)(entry + 5468) = (s16)(rsin((constVal - (s32)D_800BC060) << 6) + 4096);
                    } else {
                        *(s16 *)(entry + 5466) = 4096;
                        *(s16 *)(entry + 5468) = 4096;
                    }
                }
            }
            i++;
            flagPtr++;
            entry += 68;
        } while (i != loopCount);
    }

    if (D_800EE785 != 0) {
        D_80128A9A = 14;
    } else {
        D_80128A9A = 13;
    }
}
