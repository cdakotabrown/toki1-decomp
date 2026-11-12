/* Function: scene_entry_set */
/* Address: 0x80079104 */
/* Size: 1196 bytes (299 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x800790FC */
/* @SymbolSize: 1204 */
/* @CompileFlags: -cckr -Wc,-pic1 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:v1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "common.h"

extern void (*game_mode_step)();
extern u8 now_step;
extern u8 now_sub_step;
extern u8 now_sub_sub_step;
extern u8 scene_entry_b;
extern u8 scene_entry_c;
extern u8 step_mode;
extern u8 prev_step;
extern u8 overlay_load_main_tbl0;
extern void func_80132000();
extern void func_80132040();
extern void func_8013209C();
extern void func_80132140();
extern void func_80132214();
extern void func_80132244();
extern void func_80132274();
extern void func_801322D4();
extern void func_80132334();
extern void func_80133008();
extern void func_80133120();
extern void func_80133630();
extern void func_80133A2C();
extern void func_80134040();
extern void func_80134EC0();
extern void func_80137124();
extern void func_80137428();
extern void func_80137890();
extern void func_80137F64();
extern void func_801387E4();
extern void func_801389C0();
extern void func_80139150();
extern void func_80139700();
extern void func_8013B584();
extern void func_8013B5B0();
extern void func_8013C7A0();
extern void func_8013D258();
extern void func_80140770();
extern void func_80141450();
extern void func_80142300();
extern void func_80142600();
extern void func_80142700();
extern void func_80142960();
extern void func_8014484C();
extern void func_80144A14();
extern void func_80144F94();
extern void func_80145A00();
extern void func_80146360();
extern void func_801465E4();
extern void func_80149240();
extern void func_8014AB50();
extern void func_8014AE5C();
extern void func_8014BFF4();

void scene_entry_set()
{
    u32 mode;
    u8 e;
    u8 a;
    u8 d;
    u8 b;
    u8 c;

    mode = prev_step;
    switch (mode) {
    case 58:
        game_mode_step = func_80132000;
        break;
    case 130:
        game_mode_step = func_80132000;
        break;
    case 118:
        game_mode_step = func_80132140;
        break;
    case 117:
        game_mode_step = func_80139150;
        break;
    case 116:
        game_mode_step = func_80133A2C;
        break;
    case 128:
        game_mode_step = func_80137890;
        break;
    case 129:
        game_mode_step = func_8013B5B0;
        break;
    case 114:
        game_mode_step = func_80141450;
        break;
    case 96:
        game_mode_step = func_80132000;
        break;
    case 95:
        game_mode_step = func_80133008;
        break;
    case 115:
        game_mode_step = func_80132040;
        break;
    case 112:
        game_mode_step = func_80142700;
        break;
    case 113:
        game_mode_step = func_80144F94;
        break;
    case 17:
        game_mode_step = func_80144A14;
        break;
    case 20:
        game_mode_step = func_8013209C;
        break;
    case 21:
        game_mode_step = func_8014AE5C;
        break;
    case 19:
        game_mode_step = func_80142300;
        break;
    case 67:
        game_mode_step = func_80139700;
        break;
    case 69:
        game_mode_step = func_80132000;
        break;
    case 99:
        game_mode_step = func_80132334;
        break;
    case 196:
        game_mode_step = func_80145A00;
        break;
    case 55:
        game_mode_step = func_80149240;
        break;
    case 54:
        game_mode_step = func_8014AB50;
        break;
    case 194:
        game_mode_step = func_80142960;
        break;
    case 145:
        game_mode_step = func_80140770;
        break;
    case 195:
        game_mode_step = func_80146360;
        break;
    case 53:
        game_mode_step = func_801465E4;
        break;
    case 94:
        game_mode_step = func_8014BFF4;
        break;
    case 80:
        game_mode_step = func_8014484C;
        break;
    case 92:
        game_mode_step = func_80137F64;
        break;
    case 88:
        game_mode_step = func_80132214;
        break;
    case 98:
        game_mode_step = func_80132000;
        break;
    case 97:
        game_mode_step = func_80133120;
        break;
    case 89:
        game_mode_step = func_80132244;
        break;
    case 86:
        game_mode_step = func_80134040;
        break;
    case 87:
        game_mode_step = func_80134EC0;
        break;
    case 93:
        game_mode_step = func_801387E4;
        break;
    case 90:
        game_mode_step = func_80142600;
        break;
    case 81:
        game_mode_step = func_80137428;
        break;
    case 84:
        game_mode_step = func_80132274;
        break;
    case 82:
        game_mode_step = func_8013D258;
        break;
    case 83:
        game_mode_step = func_801322D4;
        break;
    case 85:
        game_mode_step = func_8013B584;
        break;
    case 91:
        game_mode_step = func_80137124;
        break;
    case 32:
        game_mode_step = func_80133630;
        break;
    case 33:
        game_mode_step = func_801389C0;
        break;
    case 34:
        game_mode_step = func_8013C7A0;
        break;
    default:
        game_mode_step = func_80132000;
        break;
    }
    overlay_load_main_tbl0 = 0;
    a = now_sub_sub_step;
    d = step_mode;
    c = scene_entry_c;
    b = scene_entry_b;
    now_step = prev_step; goto_sub_step(now_sub_step);
    goto_sub_sub_step(a);
    step_mode = d;
    scene_entry_c = c;
    scene_entry_b = b;
}
