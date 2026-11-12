/* Address: 0x80063B78 */
/* Size: 3316 bytes (829 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8  pad0;
    u8  mode;
    u8  flags0;
    u8  flags1;
    u8  stripW;
    u8  stripH;
    u8  pad6[6];
    u32 *ptrA;
    u32 *ptrB;
    s16 texPage;
    u8  pad22[2];
    s16 nodePtr;
    u8  pad26[12];
    s16 y1;
    u8  pad40[2];
    s16 y2;
    u8  pad44[2];
    s16 frameFlag;
    u8  pad48[4];
    u32 *ptrC;
    u32 flags2;
    u8  pad60[7];
    u8  tileFrame;
} SpriteEntry;

extern SpriteEntry class_w[];
extern u8 hud_palette_init_tbl2[];
extern u32 basyo_init_tbl1;
extern u32 basyo_init_tbl2;
extern u32 _sys_default_tpage_set_tbl2;
extern s16 basyo_init_tbl3;
extern s16 player_rec[];
extern s16 old_player_rec[];
extern s16 para_tai;
extern s16 old_para_tai;
extern s16 para_bun;
extern s16 old_para_bun;
extern s16 para_ri;
extern s16 old_para_ri;
extern s16 para_gei;
extern s16 old_para_gei;
extern s16 para_un;
extern s16 old_para_un;
extern s16 para_zat;
extern s16 old_para_zat;
extern s16 para_you;
extern s16 old_para_you;
extern s16 para_kon;
extern s16 old_para_kon;
extern s16 para_str;
extern s16 old_para_str;

extern void load_image_rect(s32, s32, s32, s32, void *);
extern void class_clear_n(s32);
extern s32 keta_n(s16, s32);
extern void memcpy(void *, void *, s32);

#define SET_DIGIT(slot, stat, pos, yy1, yy2) \
    digit = keta_n(stat, pos); class_w[slot].nodePtr = digit + 68; class_w[slot].y1 = yy1; class_w[slot].y2 = yy2

#define SET_ARROW(slot, stat, prev) \
    if (prev < stat) { \
        class_w[slot].tileFrame = class_w[slot + 1].tileFrame = class_w[slot + 2].tileFrame = 67; \
        class_w[slot].frameFlag = class_w[slot + 1].frameFlag = class_w[slot + 2].frameFlag = 1; \
    } else if (stat < prev) { \
        class_w[slot].tileFrame = class_w[slot + 1].tileFrame = class_w[slot + 2].tileFrame = 68; \
        class_w[slot].frameFlag = class_w[slot + 1].frameFlag = class_w[slot + 2].frameFlag = 1; \
    }

void parameter_show_init(void)
{
    s32 i;
    s32 j;
    s32 digit;
    s32 k;

    load_image_rect(288, 480, 16, 5, hud_palette_init_tbl2);
    for (i = 0; i < 28; i++) {
        class_clear_n(i + 32);
        class_w[i + 32].mode = 11;
        class_w[i + 32].flags0 = 64;
        class_w[i + 32].flags2 = 0x40000000;
        class_w[i + 32].flags1 = 132;
        class_w[i + 32].ptrA = &basyo_init_tbl1;
        class_w[i + 32].ptrB = &basyo_init_tbl2;
        class_w[i + 32].ptrC = &_sys_default_tpage_set_tbl2;
        class_w[i + 32].texPage = basyo_init_tbl3;
        class_w[i + 32].stripW = 8;
        class_w[i + 32].stripH = 1;
    }
    SET_DIGIT(32, para_tai, 3, -73, -88);
    SET_DIGIT(33, para_tai, 2, -65, -88);
    SET_DIGIT(34, para_tai, 1, -57, -88);
    SET_ARROW(32, para_tai, old_para_tai);
    SET_DIGIT(35, para_bun, 3, 15, -88);
    SET_DIGIT(36, para_bun, 2, 23, -88);
    SET_DIGIT(37, para_bun, 1, 31, -88);
    SET_ARROW(35, para_bun, old_para_bun);
    SET_DIGIT(38, para_ri, 3, 103, -88);
    SET_DIGIT(39, para_ri, 2, 111, -88);
    SET_DIGIT(40, para_ri, 1, 119, -88);
    SET_ARROW(38, para_ri, old_para_ri);
    SET_DIGIT(41, para_gei, 3, -73, -74);
    SET_DIGIT(42, para_gei, 2, -65, -74);
    SET_DIGIT(43, para_gei, 1, -57, -74);
    SET_ARROW(41, para_gei, old_para_gei);
    SET_DIGIT(44, para_un, 3, 15, -74);
    SET_DIGIT(45, para_un, 2, 23, -74);
    SET_DIGIT(46, para_un, 1, 31, -74);
    SET_ARROW(44, para_un, old_para_un);
    SET_DIGIT(47, para_zat, 3, 103, -74);
    SET_DIGIT(48, para_zat, 2, 111, -74);
    SET_DIGIT(49, para_zat, 1, 119, -74);
    SET_ARROW(47, para_zat, old_para_zat);
    SET_DIGIT(50, para_you, 3, -73, -60);
    SET_DIGIT(51, para_you, 2, -65, -60);
    SET_DIGIT(52, para_you, 1, -57, -60);
    SET_ARROW(50, para_you, old_para_you);
    SET_DIGIT(53, para_kon, 3, 15, -60);
    SET_DIGIT(54, para_kon, 2, 23, -60);
    SET_DIGIT(55, para_kon, 1, 31, -60);
    SET_ARROW(53, para_kon, old_para_kon);
    SET_DIGIT(56, para_str, 3, 103, -60);
    SET_DIGIT(57, para_str, 2, 111, -60);
    SET_DIGIT(58, para_str, 1, 119, -60);
    SET_ARROW(56, para_str, old_para_str);
    class_w[59].nodePtr = 61;
    class_w[59].y1 = 0;
    class_w[59].y2 = -72;
    memcpy(old_player_rec, player_rec, 124);
}
