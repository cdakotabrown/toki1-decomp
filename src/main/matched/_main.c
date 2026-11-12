/* Address: 0x80042304 */
/* Size: 404 bytes (101 instructions) */
/* Region: Game */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#ifdef PC_PORT
#include "types.h"
#else
typedef unsigned char u8;
typedef signed int s32;

extern s32 SetSp();
extern void InitHeap();
extern void bzero();
extern s32 GetGp();
void _main(void);

extern s32 boot_sp;
extern s32 boot_gp;

void main(void)
{
    boot_sp = SetSp((void*)0x801FEFF0);
    InitHeap((void*)0x801FF800, 0x7F0);
    bzero((void*)0x800ED730, 0x44698);
    boot_gp = GetGp();
    _main();
}
#endif

extern void ResetCallback(void);
extern void CdInit(void);
extern void printf(void *);
extern void input_events_init(void);
extern void ResetGraph(s32);
extern void screen_clear(void);
extern void SD_Init(void);
extern void CDQ_init(void);
extern void graph_init(void);
extern void system_reset(s32);
extern void ot_buff_set(void);
extern void game_main(void);
extern void pause_func(void);
extern void step_debug_overlay_draw(void);
extern void screen_offset_adjust(void);
extern void pn_switch(s32);
extern void vblank_function(s32);
extern void cdq_command_process(void);
extern void SD_CmdProc(void);

extern u8 _main_str0;
extern u8 screen_w[];
extern u8 pause_flag;
extern s32 frame_counter;
extern u8 _debug_number_show_str0;
extern s32 pad_data;
extern s32 pad_trig;
extern u8 sd_proc_on;

void _main(void) {
    u8 *base = screen_w;

    ResetCallback();
    CdInit();
    printf(&_main_str0);
    input_events_init();
    ResetGraph(0);
    screen_clear();

    do {
        SD_Init();
        CDQ_init();
        graph_init();
        system_reset(1);

        if (base[0x110F] != 0) {
            continue;
        }

        do {
            ot_buff_set();

            if (pause_flag == 0) {
                game_main();
            } else {
                pause_func();
            }

            frame_counter = frame_counter + 1;

            if (_debug_number_show_str0 != 0) {
                step_debug_overlay_draw();
                if ((pad_data & 0xC0000) == 0xC0000) {
                    screen_offset_adjust();
                }
                if (pad_trig & 0xA0000) {
                    pn_switch(1);
                }
            }

            if (base[0x110F] == 0) {
                vblank_function(1);
                cdq_command_process();
                if (sd_proc_on != 0) {
                    SD_CmdProc();
                }
            }
        } while (base[0x110F] == 0);
    } while (1);
}
