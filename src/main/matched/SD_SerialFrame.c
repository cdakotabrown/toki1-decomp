/* Function: SD_SerialFrame */
/* Address: 0x80079F90 */
/* Size: 264 bytes (66 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s32 get_CD_State(void);
extern void serial_cmd(s32 a0);
extern void SD_SerialClear(void);
extern void SD_SerialVolSend(s32 a0);
extern void SsSetMVol(s32 a0, s32 a1);
extern void SsSetSerialVol(s32 a0, s16 a1, s16 a2);
extern void SsSetSerialAttr(s32 a0, s32 a1, s32 a2);

typedef struct {
    s32 field_00;
    s32 field_04;
    s32 field_08;
    s32 field_0C;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    s16 field_1C;
    s16 field_1E;
} D_8012DD80Struct;

extern D_8012DD80Struct gSDInfo_frame;

void SD_SerialFrame(void) {
    D_8012DD80Struct *p = &gSDInfo_frame;
    s32 temp_v0;

    if (get_CD_State() & 4) {
        p->field_04 = p->field_04 & ~0x200;
        if (p->field_10 != 0) {
            serial_cmd(116);
            SD_SerialClear();
            temp_v0 = p->field_00;
            if (!(temp_v0 & 0x4000)) {
                SD_SerialVolSend(p->field_08);
            }
        } else {
            temp_v0 = p->field_00;
            if (!(temp_v0 & 0x4000)) {
                p->field_1C = p->field_1E;
                p->field_00 = (temp_v0 | 0x400) & 0xFFEFFFFF;
                SsSetMVol(127, 127);
                SsSetSerialVol(0, p->field_1C, p->field_1C);
                SsSetSerialAttr(0, 0, 1);
            }
        }
    }
}
