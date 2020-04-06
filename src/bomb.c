#include "bomb.h"

#include <stdlib.h>
#include <string.h>

#include <assert.h>

#include "input.h"
#include "util.h"

struct bomb {
    int batteries;
    char serial[BOMB_SERIAL_MAX_LENGTH];

    struct feature {
        int active;
        char name[16];
    } features[16];
    int features_count;
};

struct bomb* bomb_create()
{
    struct bomb* b = (struct bomb*) malloc(sizeof(struct bomb));
    bomb_reset(b);
    return b;
}

void bomb_destroy(bomb_t *bomb)
{
    free(bomb);
}

void bomb_reset(struct bomb* bomb)
{
    bomb->batteries = -1;
    bomb->serial[0] = 0;
    bomb->features_count = 0;
}

static inline int bomb_serial_is_set(struct bomb* bomb)
{
    return (bomb->serial[0] != 0);
}

static char* bomb_get_serial(struct bomb* bomb)
{
    if (!bomb_serial_is_set(bomb)) {
        prompt_line("What is the bombs serial number?", bomb->serial, BOMB_SERIAL_MAX_LENGTH);
    }
}

int bomb_serial_has_vowel(struct bomb* bomb)
{
    int i = 0;
    char * serial = bomb_get_serial(bomb);

    for (; serial[i]; i++) {
        if (contains("aeiouyAEIOUY", serial[i])) {
            return 1;
        }
    }

    return 0;
}

int bomb_serial_ends_odd(struct bomb* bomb)
{
    char* serial = bomb_get_serial(bomb);
    size_t last = strlen(serial) - 1;
    assert(last != 0);

    return ((bomb->serial[last] - '0') % 2 == 1);
}

int bomb_serial_ends_even(struct bomb* bomb)
{
    return !bomb_serial_ends_odd(bomb);
}

int bomb_has_feature(struct bomb* bomb, char* name)
{
    int i = 0;
    int has;

    for (; i < bomb->features_count; i++) {
        if (strcmp(bomb->features[i].name, name) == 0) {
            return bomb->features[i].active;
        }
    }

    has = (prompt_yesno("Does the bomb have a parallel port?"));
    bomb_set_feature(bomb, "parallel", has);
    return has;
}

int bomb_set_feature(struct bomb* bomb, char* name, int active)
{
    strcpy(bomb->features[bomb->features_count].name, name);
    bomb->features[bomb->features_count].active = active;
    bomb->features_count++;
}

int bomb_battery_count(struct bomb* bomb)
{
    if (bomb->batteries == -1) {
        bomb->batteries = prompt_digit("How many batteries does the bomb have?", 0, 9);
    }

    return bomb->batteries;
}