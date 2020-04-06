#ifndef BOMB_H_
#define BOMB_H_

#define BOMB_SERIAL_MAX_LENGTH 16

typedef struct bomb bomb_t;

bomb_t* bomb_create();
void bomb_destroy(bomb_t *bomb);

void bomb_reset(bomb_t *bomb);

int bomb_serial_has_vowel(bomb_t *bomb);
int bomb_serial_ends_odd(bomb_t *bomb);
int bomb_serial_ends_even(bomb_t *bomb);

int bomb_has_feature(bomb_t *bomb, char* name);
int bomb_set_feature(bomb_t *bomb, char* name, int active);

int bomb_battery_count(bomb_t * bomb);

#endif