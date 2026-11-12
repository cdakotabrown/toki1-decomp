#ifndef TOKIMEKI_RPG_BAT_UNIT_H
#define TOKIMEKI_RPG_BAT_UNIT_H

/* RPG_BAT battle unit records: rpg_bat_unit0..3 (0x8015ED94, EDC0, EDEC,
 * EE18), four separate objects of eleven words.  hp / flag / hp_max are
 * the combatant state (flag's low byte non-zero = busy; tested for all
 * four); step / sub / cnt / wait are the step machine of
 * rpg_bat_step_next / rpg_bat_state_next / rpg_bat_timer_step; disp is
 * the sprite-display word. */

typedef struct {
    int hp;         /* 0x00 */
    int flag;       /* 0x04 */
    int aux08;      /* 0x08 */
    int hp_max;     /* 0x0C */
    int aux10;      /* 0x10 */
    int aux14;      /* 0x14 */
    int step;       /* 0x18 */
    int sub;        /* 0x1C */
    int cnt;        /* 0x20 */
    int wait;       /* 0x24 */
    int disp;       /* 0x28 */
} RPG_BAT_UNIT;

typedef char rpg_bat_unit_size_check[sizeof(RPG_BAT_UNIT) == 44 ? 1 : -1];

#endif
