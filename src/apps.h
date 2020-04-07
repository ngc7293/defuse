/** apps.h
 * @author    David Bourgault
 * @copyright no
 * @brief     Contains function definition for all application functions.
 * 
 * New functions should be declared here and added to the `apps[]` list.
 *
 * An application function corresponds to a function capable of resolving a
 * complete bomb module before returning. It returns either EXIT_SUCCESS or
 * EXIT_FAILURE, and accepts a list of string arguments (although this is
 * currently unused.)
 * 
 * There is no need to #include this file in any files except main.c. Doing so
 * _will_ cause compilation errors.
 */
#ifndef APPS_H_
#define APPS_H_

#include <stdlib.h>

#include "bomb.h"

/* Commands */
int app_help(bomb_t *bomb);
int app_reset(bomb_t *bomb);

/* Modules */
int app_complex_wires(bomb_t *bomb);
int app_password(bomb_t *bomb);
int app_sequence(bomb_t* bomb);
int app_wires(bomb_t *bomb);

struct app {
    const char* name;
    int (*function)(bomb_t *);
} apps[] = {
    { "help", app_help },
    { "reset", app_reset },
    { "complex_wires", app_complex_wires },
    { "wires2", app_complex_wires },
    { "password", app_password },
    { "pass", app_password },
    { "sequence", app_sequence },
    { "wires", app_wires },
    { NULL, NULL }
};

#endif