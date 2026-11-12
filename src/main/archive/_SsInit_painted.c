/* Function: _SsInit */
/* Address: 0x8008B388 */
/* Size: 424 bytes (106 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -fno-strength-reduce --fill-all */
/* @PostProc: custom_script custom_func_8008B388.py */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

extern s16 libsnd_spu_voice_reset_tbl[8];
extern s16 libsnd_spu_ctrl_reset_tbl[16];
extern s32 VBLANK_MINUS;
extern s32 _snd_openflag;
extern s32 _snd_ev_flag;
extern s32 D_800EB714;
extern s32 libcd_timeout_deadline;
extern s32 libcd_timeout_spin_count;
extern s32 libcd_cur_func_name;
extern s32 _snd_video_mode;
extern s32 _snd_vsync_cb;

void ResetCallback(void);
void SpuInit(void);
void SpuInitHot(void);
void SpuVmInit(s32 a0);
s32 GetVideoMode(void);

void _SsInit(s32 arg0) {
    s32 i;
    volatile s16 *dest;
    s16 *src;
    s32 j;
    u32 *clearOuter;
    u32 *clearEnd;
    u32 *clearPtr;
    (void)&arg0;

    ResetCallback();

    if (arg0 != 0) {
        SpuInitHot();
    } else {
        SpuInit();
    }

    dest = (volatile s16 *)0x1F801C00;
    i = 0;
    do {
        src = libsnd_spu_voice_reset_tbl;
        do {
            *dest++ = src[0];
            *dest++ = src[1];
            *dest++ = src[2];
            *dest++ = src[3];
            src += 4;
        } while (src < libsnd_spu_voice_reset_tbl + 8);
        i++;
    } while (i < 24);

    dest = (volatile s16 *)0x1F801D80;
    src = libsnd_spu_ctrl_reset_tbl;
    do {
        *dest++ = src[0];
        *dest++ = src[1];
        *dest++ = src[2];
        *dest++ = src[3];
        src += 4;
    } while (src < libsnd_spu_ctrl_reset_tbl + 16);

    SpuVmInit(24);

    clearOuter = (u32 *)0x80130B80;
    clearEnd = (u32 *)0x80131380;
    do {
        clearPtr = clearOuter;
        j = 0;
        do {
            clearPtr[0] = 0;
            j += 4;
            clearPtr[1] = 0;
            clearPtr[2] = 0;
            clearPtr[3] = 0;
            clearPtr += 4;
        } while (j != 16);
        clearOuter += 16;
    } while (clearOuter < clearEnd);

    VBLANK_MINUS = 60;
    _snd_openflag = 0;
    D_800EB714 = 0;
    libcd_timeout_spin_count = -1;
    libcd_timeout_deadline = 0;
    libcd_cur_func_name = 0;
    _snd_vsync_cb = 0;
    GetVideoMode();
    _snd_video_mode = 0;
    _snd_ev_flag = 0;
}
