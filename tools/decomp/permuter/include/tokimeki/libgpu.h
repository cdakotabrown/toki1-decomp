/**
 * Tokimeki Memorial - Custom libgpu.h
 *
 * This header overrides standard PSX SDK types with Tokimeki-specific versions.
 * DRAWENV in Tokimeki Memorial is 96 bytes, not the standard 92 bytes.
 */

#ifndef TOKIMEKI_LIBGPU_H
#define TOKIMEKI_LIBGPU_H

/* Standard PSX types */
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned long u_long;
typedef signed char s_char;
typedef signed short s_short;
typedef signed long s_long;

/* RECT - Standard 8 bytes */
typedef struct {
    short x, y, w, h;
} RECT;

/* DR_ENV - Drawing environment tag (64 bytes) */
typedef struct {
    u_long tag;
    u_long code[15];
} DR_ENV;

/**
 * DRAWENV - Tokimeki Memorial version (96 bytes)
 *
 * The standard PSX SDK DRAWENV is 92 bytes. Tokimeki Memorial uses a
 * version with 4 extra bytes of padding at the end.
 *
 * Evidence: Stack frame analysis of draw_and_disp_env_init (0x800410AC)
 * shows draw struct at sp+64 with 160-byte frame, which only works
 * with a 96-byte DRAWENV.
 */
typedef struct {
    RECT clip;              /* Clipping area */
    short ofs[2];           /* Drawing offset */
    RECT tw;                /* Texture window */
    u_short tpage;          /* Texture page */
    u_char dtd;             /* Dither flag */
    u_char dfe;             /* Drawing to display area flag */
    u_char isbg;            /* Background clear flag */
    u_char r0, g0, b0;      /* Background color */
    DR_ENV dr_env;          /* Drawing environment primitive */
    int _tokimeki_pad;      /* Tokimeki-specific: 4 extra bytes */
} DRAWENV;

/* Verify DRAWENV is exactly 96 bytes */
#ifdef __STDC_VERSION__
#if __STDC_VERSION__ >= 201112L
_Static_assert(sizeof(DRAWENV) == 96, "DRAWENV must be 96 bytes for Tokimeki Memorial");
#endif
#endif

/**
 * DISPENV - Display environment (20 bytes) - Standard size
 */
typedef struct {
    RECT disp;              /* Display area */
    RECT screen;            /* Screen area */
    u_char isinter;         /* Interlace flag */
    u_char isrgb24;         /* 24-bit color flag */
    u_char pad0;            /* Padding */
    u_char pad1;            /* Padding */
} DISPENV;

/* PSX GPU functions */
extern void GetDrawEnv(DRAWENV *env);
extern void PutDrawEnv(DRAWENV *env);
extern void GetDispEnv(DISPENV *env);
extern void PutDispEnv(DISPENV *env);
extern void SetDefDrawEnv(DRAWENV *env, int x, int y, int w, int h);
extern void SetDefDispEnv(DISPENV *env, int x, int y, int w, int h);
extern void ClearImage(RECT *rect, u_char r, u_char g, u_char b);
extern void LoadImage(RECT *rect, u_long *data);
extern void StoreImage(RECT *rect, u_long *data);
extern void MoveImage(RECT *rect, int x, int y);
extern void DrawSync(int mode);
extern int VSync(int mode);

#endif /* TOKIMEKI_LIBGPU_H */
