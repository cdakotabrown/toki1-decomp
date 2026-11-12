/* Likely Symbol: bustup_cd_read (from o.bin, offset +228 bytes from 0x80062EB0) */
/* Address: 0x80062DCC */
/* Size: 156 bytes */
/* Compiler: IDO 5.2 (compiled) for GCC region (0x80040000-0x80080000) */
typedef int s32;
typedef unsigned int u32;

extern void set_movie_offset(s32 a0, s32 a1);
extern void movie_main(s32 a0, s32 a1, s32 a2);
extern void StUnSetRing(void);
extern void cd_command_issue(s32 a0, s32 a1, s32 a2);
extern void dec_env_set(void *a0, void *a1, void *a2);
extern void ResetGraph(s32 a0);
extern void vram_area_clear(s32 a0);
extern void DrawSync(s32 a0);
extern void back_clear_switch(s32 a0);
extern void goto_step(s32 a0);

void movie_play_main(void) {
    set_movie_offset(0, 0);
    movie_main(0, 167, 400);
    StUnSetRing();
    cd_command_issue(9, 0, 0);
    cd_command_issue(10, 0, 0);
    dec_env_set((void *)0x80167000, (void *)0x80162000, (void *)0x80162000);
    ResetGraph(1);
    vram_area_clear(0);
    DrawSync(0);
    back_clear_switch(1);
    goto_step(0);
}
