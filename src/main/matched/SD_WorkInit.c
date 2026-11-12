/* Function: SD_WorkInit */
/* Debug Symbol: SD_WorkInit (from o.bin, high match) */
/* Address: 0x8007C088 */
/* Size: 56 bytes (14 instructions) */
typedef signed short s16;

extern s16 gSDCDLevelInfo[];

void SD_WorkInit(void) {
    gSDCDLevelInfo[0] = 0;
    gSDCDLevelInfo[1] = 0;
    gSDCDLevelInfo[2] = 8;
    gSDCDLevelInfo[3] = 32;
    gSDCDLevelInfo[4] = 530;
}
