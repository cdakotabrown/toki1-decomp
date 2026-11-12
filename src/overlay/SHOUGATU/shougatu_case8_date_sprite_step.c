/* Address: 0x80143164 */
/* Size: 352 bytes (88 instructions) */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:7 */
#include "types.h"
#include "tokimeki/class_w.h"

typedef struct {
    u8 b[192];
} StepTbl;

extern StepTbl shougatu_main8_tbl;
extern s32 shougatu_cmd_send(void);
extern u32 frame_counter;
extern u8 now_sub_sub_step;
typedef struct {
    u8 pad0[7];
    u8 b37;
    s16 b38;
    u8 pad1[12];
    s16 b46;
    s16 b48;
    u8 pad2[0x70];
    u8 bba;
    u8 bbb;
    u8 pad3[3];
    u8 bbf;
} Blk30;
extern s32 date_sprite_on;
extern u8 fade_bri;
extern CLASS_W class_w[];

s32 shougatu_case8_date_sprite_step(void)
{
    s32 cnt;
    StepTbl tbl;
    s32 ret;

    tbl = shougatu_main8_tbl;
    ret = (*(s32 (**)(s32))(tbl.b + now_sub_sub_step * 4))(60);

    if (now_sub_sub_step < 17U) {
        (*(Blk30 *)&class_w[96]).b37 = fade_bri;
        if ((*(Blk30 *)&class_w[96]).b48 == 0) {
            if ((*(Blk30 *)&class_w[96]).b38 == 0) {
                cnt = (*(Blk30 *)&class_w[96]).b46 + 1;
                cnt = (cnt << 16) >> 16;
                (*(Blk30 *)&class_w[96]).b46 = (s16)cnt;
                if (cnt >= 3) {
                    (*(Blk30 *)&class_w[96]).b46 = 0;
                    ret = 0;
                }
            }
        }
    }
    if (date_sprite_on != 0) {
        (*(Blk30 *)&class_w[96]).bbb |= 0x80;
        (*(Blk30 *)&class_w[96]).bbf = (*(Blk30 *)&class_w[96]).b37;
        if (frame_counter % 300 == 0) {
            ret = shougatu_cmd_send();
            (*(Blk30 *)&class_w[96]).bba = 3;
        }
    } else {
        (*(Blk30 *)&class_w[96]).bbb &= 0xFF7F;
    }
    return ret;
}
