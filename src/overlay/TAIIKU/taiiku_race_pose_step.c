/* @CompileEnv: IDO52_COPYPROP_SITE=lod:1:deny,lod:2:deny IDO52_COPYPROP_VAR=-4@7:1 */
/* @SymbolStart: 0x80143F7C */
/* @SymbolSize: 384 */
/* @CompileFlags: -cckr */
#include "types.h"

extern unsigned int taiiku_ninin_tempo;
extern unsigned int taiiku_ninin_run_0_tempo_cnt;
extern int taiiku_ninin_run_0_run_flag;
extern unsigned char taiiku_ninin_run;
extern unsigned char taiiku_ninin_girl1;
extern unsigned char taiiku_ninin_run_0_pose;
extern short cw100_frame;
extern short cal_cw_frame[];
extern short taiiku_ninin_pfrm_tbl[];
extern short taiiku_ninin_pfrm_tbl1[];
extern short cal_cw_x;
extern short cal_cw_y;
extern short cw100_x;
extern short cw100_y;

void taiiku_race_pose_step()
{
    int v;

    if (taiiku_ninin_tempo < taiiku_ninin_run_0_tempo_cnt && taiiku_ninin_run_0_run_flag != 0) {
        v = taiiku_ninin_run;
        if (v >= 9) {
            v = 2;
            taiiku_ninin_run_0_tempo_cnt = 0;
        } else {
            v = (unsigned char)(v + 1);
            taiiku_ninin_run_0_tempo_cnt = 0;
        }
        taiiku_ninin_run = v;
        if (v == 6) {
            cw100_frame = 46;
            if (taiiku_ninin_girl1 != 0) {
                device_cmd_send(2052);
            } else {
                device_cmd_send(2049);
            }
            taiiku_ninin_run_0_pose = 1;
        }
        v = taiiku_ninin_run;
        if (v == 2) {
            cw100_frame = 47;
            if (taiiku_ninin_girl1 != 0) {
                device_cmd_send(2053);
            } else {
                device_cmd_send(2050);
            }
        }
    }
    v = taiiku_ninin_run;
    if (taiiku_ninin_run_0_run_flag != 0) {
        if (taiiku_ninin_girl1 != 0) {
            cal_cw_frame[0] = taiiku_ninin_pfrm_tbl1[v];
            taiiku_ninin_run = v;
        } else {
            cal_cw_frame[0] = taiiku_ninin_pfrm_tbl[v];
            taiiku_ninin_run = v;
        }
    }
    cal_cw_frame[143] = cal_cw_frame[7];
    cal_cw_frame[145] = cal_cw_frame[9];
}
