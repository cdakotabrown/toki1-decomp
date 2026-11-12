/* @CompileEnv: IDO52_FORCE_WEB_COLOR=11:v1 IDO52_AS1_NO_BRANCH_FOLD=4,5,6,7,8,9,10,11,12,13,14,15 IDO52_PRECOLOR_WEB=a3:23 */
/* @SymbolStart: 0x80133A64 */
/* @SymbolSize: 436 */
/* @CompileFlags: -cckr */
/* kangei_bukatu_para_set - KANGEI overlay, Address: 0x80133A6C, Size: 428 bytes */
#include "types.h"

extern unsigned char cal_cw_mode;
extern short cal_cw_tick;
extern short cal_cw_anim;
extern volatile unsigned short basyo_pack;
extern unsigned char schedule_cursor;
extern unsigned char screen_w[];
extern s32 para_fixed[];

void kangei_bukatu_para_set()
{
    int a;
    int b;
    int c;
    int t;
    unsigned int i;
    unsigned char *p;
    unsigned char *q;

    if (cal_cw_mode & 1) {
        return;
    }
    if (cal_cw_tick == 0) {
        switch (cal_cw_anim & 1) {
        case 0:
            device_cmd_send(1537);
            break;
        case 1:
            device_cmd_send(1538);
            break;
        }
        p = screen_w + schedule_cursor * 4;
        a = p[1693];
        b = p[1694];
        q = screen_w + (basyo_pack >> 12) * 4;
        *(unsigned short *)(q + 1644) += 2;
        c = 0;
        i = basyo_pack >> 12;
        if (i < 2) {
            ((int *)&para_fixed[1])[0] += 0x18000;
        } else if (i < 4) {
            ((int *)&para_fixed[1])[1] += 0x18000;
        } else if (i < 6) {
            ((int *)&para_fixed[1])[2] += 0x18000;
        } else {
            ((int *)&para_fixed[1])[3] += 0x18000;
            ((int *)&para_fixed[1])[6] += 0x18000;
        }
        parameter_up_down(a, c, b, a);
        parameter_show_init();
    }
    t = cal_cw_tick++ >= 5;
    if (t) {
        sub_step_up();
    }
}
