/* Address: 0x8007AF50 - 0x8007AF9B (76 bytes) */
/* Region: GCC (0x80041000 - 0x80080FFF) */
typedef signed short s16;
typedef signed int s32;

extern s32 gSDSerialInfo;
extern s16 sd_vab_ch0;

extern s32 seq_busy_check(s16 arg0);
extern void seq_start(void);

void seq_start_req(void)
{
    if (seq_busy_check(sd_vab_ch0) == 0) {
        gSDSerialInfo = gSDSerialInfo | 2;
    } else {
        seq_start();
    }
}
