/* Address: 0x80090DA8 */
/* Size: 724 bytes (181 instructions) */
/* Region: IDO (0x80081000-0x800BFFFF) */
#include "common.h"

extern void SpuVmFlush(void);
extern void Snd_stop(void);
extern void SsSeqCalledTbyT(void);
extern void Snd_play(void);
extern void Snd_pause(void);
extern void Snd_replay(void);
extern void Snd_crescendo(void);
extern void Snd_decrescendo(void);
extern void Snd_tempo(void);
extern void func_8009DDC0(void);
extern void func_800A02A0(void);
extern void func_800A19E0(void);
extern void func_800A1A10(void);
extern void func_800A1B20(void);
extern void func_800A1BA0(void);
extern void func_800A2170(void);
extern void func_800A27B0(void);

extern void SsSeqCalledTbyT(void);
extern void func_8009DDC0(void);
extern void func_800A02A0(void);
extern void func_800A19E0(void);
extern void func_800A1A10(void);
extern void func_800A1B20(void);
extern void func_800A1BA0(void);
extern void func_800A2170(void);
extern void func_800A27B0(void);

#if 0
void SsSeqCalledTbyT(void) {
    s32 var_s0;
    s32 var_s2;
    s32 var_t2;
    s32 var_t4;
    s32 var_t6;
    s32 var_t6_2;
    s32 var_t6_3;
    s32 var_t8;
    s32 var_t9;
    s32 var_v0;
    s32* temp_s1;

        *(s32* )0x8013140C = 1;
        SpuVmFlush();
        var_s2 = 0;
        if (*(s16* )0x80131408 > 0) {
            do {
                if (*(s32* )0x80131404 & (1 << var_s2)) {
                    var_s0 = 0;
                    if (*(void* )0x8013140A > 0) {
                        temp_s1 = (var_s2 * 4) + 0x80131380;
                        var_t6 = 0 * 4;
                        do {
                            var_v0 = (*temp_s1 + ((((var_t6 + var_s0) * 4) + var_s0) * 8))->unk90;
                            var_t8 = var_v0 & 2;
                            if (var_v0 & 1) {
                                Snd_play(var_s2, var_s0);
                                var_v0 = (*temp_s1 + (var_s0 * 0xA8))->unk90;
                                var_t6_2 = var_v0 & 0x20;
                                if (var_v0 & 0x10) {
                                    Snd_crescendo(var_s2, var_s0);
                                    var_v0 = (*temp_s1 + (var_s0 * 0xA8))->unk90;
                                    var_t6_2 = var_v0 & 0x20;
                                }
                                var_t9 = var_v0 & 0x40;
                                if (var_t6_2 != 0) {
                                    Snd_decrescendo(var_s2, var_s0);
                                    var_v0 = (*temp_s1 + (var_s0 * 0xA8))->unk90;
                                    var_t9 = var_v0 & 0x40;
                                }
                                var_t4 = var_v0 & 0x80;
                                if (var_t9 != 0) {
                                    Snd_tempo(var_s2, var_s0);
                                    var_v0 = (*temp_s1 + (var_s0 * 0xA8))->unk90;
                                    var_t4 = var_v0 & 0x80;
                                }
                                var_t8 = var_v0 & 2;
                                if (var_t4 != 0) {
                                    Snd_tempo(var_s2, var_s0);
                                    var_v0 = (*temp_s1 + (var_s0 * 0xA8))->unk90;
                                    var_t8 = var_v0 & 2;
                                }
                            }
                            var_t2 = var_v0 & 8;
                            if (var_t8 != 0) {
                                Snd_pause(var_s2, var_s0);
                                var_v0 = (*temp_s1 + (var_s0 * 0xA8))->unk90;
                                var_t2 = var_v0 & 8;
                            }
                            var_t6_3 = var_v0 & 4;
                            if (var_t2 != 0) {
                                Snd_replay(var_s2, var_s0);
                                var_t6_3 = (*temp_s1 + (var_s0 * 0xA8))->unk90 & 4;
                            }
                            if (var_t6_3 != 0) {
                                Snd_stop(var_s2, var_s0);
                                (*temp_s1 + (var_s0 * 0xA8))->unk90 = 0;
                            }
                            var_s0 += 1;
                            var_t6 = var_s0 * 4;
                        } while (var_s0 < *(void* )0x8013140A);
                    }
                }
                var_s2 += 1;
            } while (var_s2 < *(void* )0x80131408);
        }
        *(void* )0x8013140C = 0;
    }
}
#endif

void SsSeqCalledTbyT(void) {

}
