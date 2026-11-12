/* Function: girl_season_load */
/* Address: 0x80083D34 */
/* Size: 1572 bytes (393 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x80083D2C */
/* @SymbolSize: 1580 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_trailing_instructions --count 1 --nop-only */
#include "types.h"

extern u8 now_girl;
extern u8 now_month;

extern void bustup_cd_read(s32);
extern void sub_sub_step_up(void);

void girl_season_load(void) {
    u32 month;

    switch (now_girl) {
    case 0:
        month = now_month;
        if (month < 3U) {
        } else if (month < 6U) {
            bustup_cd_read(21938);
            break;
        }
        if (month < 6U) {
        } else if (month < 9U) {
            bustup_cd_read(21983);
            break;
        }
        if (month < 9U) {
        } else if (month < 12U) {
            bustup_cd_read(22028);
            break;
        }
        bustup_cd_read(22073);
        break;

    case 1:
        month = now_month;
        if (month < 3U) {
        } else if (month < 6U) {
            bustup_cd_read(23963);
            break;
        }
        if (month < 6U) {
        } else if (month < 9U) {
            bustup_cd_read(24008);
            break;
        }
        if (month < 9U) {
        } else if (month < 12U) {
            bustup_cd_read(24053);
            break;
        }
        bustup_cd_read(24098);
        break;

    case 2:
        month = now_month;
        if (month < 3U) {
        } else if (month < 6U) {
            bustup_cd_read(21128);
            break;
        }
        if (month < 6U) {
        } else if (month < 9U) {
            bustup_cd_read(21173);
            break;
        }
        if (month < 9U) {
        } else if (month < 12U) {
            bustup_cd_read(21218);
            break;
        }
        bustup_cd_read(21263);
        break;

    case 3:
        month = now_month;
        if (month < 3U) {
        } else if (month < 6U) {
            bustup_cd_read(22793);
            break;
        }
        if (month < 6U) {
        } else if (month < 9U) {
            bustup_cd_read(22838);
            break;
        }
        if (month < 9U) {
        } else if (month < 12U) {
            bustup_cd_read(22883);
            break;
        }
        bustup_cd_read(22928);
        break;

    case 4:
        month = now_month;
        if (month < 3U) {
        } else if (month < 6U) {
            bustup_cd_read(26393);
            break;
        }
        if (month < 6U) {
        } else if (month < 9U) {
            bustup_cd_read(26438);
            break;
        }
        if (month < 9U) {
        } else if (month < 12U) {
            bustup_cd_read(26483);
            break;
        }
        bustup_cd_read(26528);
        break;

    case 5:
        month = now_month;
        if (month < 3U) {
        } else if (month < 6U) {
            bustup_cd_read(24593);
            break;
        }
        if (month < 6U) {
        } else if (month < 9U) {
            bustup_cd_read(24638);
            break;
        }
        if (month < 9U) {
        } else if (month < 12U) {
            bustup_cd_read(24683);
            break;
        }
        bustup_cd_read(24728);
        break;

    case 6:
        month = now_month;
        if (month < 3U) {
        } else if (month < 6U) {
            bustup_cd_read(25223);
            break;
        }
        if (month < 6U) {
        } else if (month < 9U) {
            bustup_cd_read(25268);
            break;
        }
        if (month < 9U) {
        } else if (month < 12U) {
            bustup_cd_read(25313);
            break;
        }
        bustup_cd_read(25358);
        break;

    case 7:
        month = now_month;
        if (month < 3U) {
        } else if (month < 6U) {
            bustup_cd_read(23378);
            break;
        }
        if (month < 6U) {
        } else if (month < 9U) {
            bustup_cd_read(23423);
            break;
        }
        if (month < 9U) {
        } else if (month < 12U) {
            bustup_cd_read(23468);
            break;
        }
        bustup_cd_read(23513);
        break;

    case 8:
        month = now_month;
        if (month < 3U) {
        } else if (month < 6U) {
            bustup_cd_read(20543);
            break;
        }
        if (month < 6U) {
        } else if (month < 9U) {
            bustup_cd_read(20588);
            break;
        }
        if (month < 9U) {
        } else if (month < 12U) {
            bustup_cd_read(20633);
            break;
        }
        bustup_cd_read(20678);
        break;

    case 9:
        month = now_month;
        if (month < 3U) {
        } else if (month < 6U) {
            bustup_cd_read(25853);
            break;
        }
        if (month < 6U) {
        } else if (month < 9U) {
            bustup_cd_read(25898);
            break;
        }
        if (month < 9U) {
        } else if (month < 12U) {
            bustup_cd_read(25943);
            break;
        }
        bustup_cd_read(25988);
        break;

    case 10:
        month = now_month;
        if (month < 3U) {
        } else if (month < 6U) {
            bustup_cd_read(27023);
            break;
        }
        if (month < 6U) {
        } else if (month < 9U) {
            bustup_cd_read(27068);
            break;
        }
        if (month < 9U) {
        } else if (month < 12U) {
            bustup_cd_read(27113);
            break;
        }
        bustup_cd_read(27158);
        break;

    case 13:
        bustup_cd_read(27788);
        break;
    }
    sub_sub_step_up();
}
