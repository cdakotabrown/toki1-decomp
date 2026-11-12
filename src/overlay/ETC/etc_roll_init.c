/* @SymbolSize: 292 */
/* @PostProc: patch_insn_reg_field 0x14:rt:v0:a0 0x18:rs:v0:a0 0x18:rt:v0:a0 0x3c:rt:a0:v1 0x50:rt:v0:a0 0x68:rs:a0:v1 0x68:rt:a0:v1 0x90:rt:v1:v0 0x9c:rs:a0:v1 0xa0:rs:a0:v1 0xac:rs:a0:v1 0xb0:rs:a0:v1 0xb0:rt:v1:v0 0xb4:rs:a0:v1 0xb8:rs:a0:v1 0xbc:rs:a0:v1 0xc0:rs:a0:v1 0xc4:rs:a0:v1 0xc8:rs:a0:v1 0xcc:rs:a0:v1 0xd0:rs:a0:v1 0xd4:rs:a0:v1 0xd8:rs:a0:v1 0xdc:rs:a0:v1 0xe0:rs:a0:v1 0xe8:rs:a0:v1 0xfc:rs:a0:v1 0xfc:rt:a0:v1 0x104:rs:v0:a0 0x104:rt:v0:a0 0x128:rt:v0:a0 */
/* Address: 0x801436D8 */
/* Size: 292 bytes (73 instructions) */
#include "types.h"

typedef struct {
      u8 b0;
      u8 b1;
      u8 b2;
      u8 b3;
      u8 b4;
      u8 b5;
      u8 b6;
      u8 b7;
      s32 w8;
      s32 w12;
      s32 w16;
      s32 w20;
      s16 h24;
      s16 h26;
      s16 h28;
      s16 h30;
      s16 h32;
      s16 h34;
      s16 h36;
      s16 h38;
      s16 h40;
      s16 h42;
      s16 h44;
      s16 h46;
      s32 w48;
      s32 w52;
      s32 w56;
      s32 w60;
      u8 b64;
      u8 b65;
      u8 b66;
      u8 b67;
} EtcSlot;

extern EtcSlot class_w[];
extern EtcSlot cw96_active[];
extern u8 etc_roll_tbl[];
extern s32 etc_roll_idx;

void etc_roll_init(void)
{
    s32 idx;
    s32 j;

    idx = etc_roll_idx;
    for (j = 0; j < 7; j++) {
        class_w[j + 96].h24 = (s16)etc_roll_tbl[idx * 2];
        class_w[j + 96].b1 = 0;
        class_w[j + 96].b2 = 64;
        class_w[j + 96].w56 = 0;
        class_w[j + 96].b3 = 164;
        class_w[j + 96].w12 = 0x801F86EC;
        class_w[j + 96].w16 = 0x801F897C;
        class_w[j + 96].w52 = 0x801F86A4;
        class_w[j + 96].b4 = 8;
        class_w[j + 96].b67 = 15;
        class_w[j + 96].b5 = 128;
        class_w[j + 96].h38 = (s16)(etc_roll_tbl[idx * 2 + 1] - 160);
        class_w[j + 96].h42 = (s16)(140 + j * 40);
        class_w[j + 96].h46 = 0;
        class_w[j + 96].w48 = -1;
        class_w[j + 96].b7 = 128;
        if (class_w[j + 96].h24 >= 241) {
            cw96_active[j].b3 = 52;
        } else {
            cw96_active[j].b3 = 180;
        }
        idx++;
    }
    etc_roll_idx = idx;
}
