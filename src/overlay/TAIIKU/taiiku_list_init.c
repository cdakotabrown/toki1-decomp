/* @SymbolSize: 192 */
/* /* @PostProc: patch_insn_reg_field 0x5c:rd:v0:s0 0x68:rs:v0:s0 0x70:rs:v0:s0 */
/* Address: 0x80139798 */
/* Size: 192 bytes (48 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 sub_step_up(void);
extern s32 device_cmd_send(s32 a0);
extern s32 taiiku_run_speed(void);
extern void taiiku_course_disp(void);
extern u8 get_taiiku_leader2(void);

extern u8  taiiku_tama_rank[4];
extern s32 taiiku_tama_scale;
extern CLASS_W class_w[];

void taiiku_list_init(void)
{
    s32 v1;
    u8 *s0;
    u8 *s1;

    device_cmd_send(1284);

    v1 = 0;
    if (taiiku_tama_rank[0] != 255) {
        s0 = &taiiku_tama_rank[0];
        do {
            v1++;
            s0++;
        } while (s0[0] != 255);
    }

    s0 = &taiiku_tama_rank[v1];
    s0[0] = 0;
    s0++;
    v1++;

    if (v1 < 4) {
        s1 = &taiiku_tama_rank[4];
        do {
            *s0++ = get_taiiku_leader2();
        } while (s0 < s1);
    }

    taiiku_course_disp();
    taiiku_run_speed();
    taiiku_tama_scale = 4608;
    class_w[103].disp_flag = 0;
    sub_step_up();
}
