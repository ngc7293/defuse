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

int app_exit(char* args[]);
int app_help(char* args[]);
int app_complex_wires(char* args[]);
int app_password(char* args[]);

struct app {
    const char* name;
    int (*function)(char*[]);
} apps[] = {
    { "exit", app_exit },
    { "help", app_help },
    { "complex_wires", app_complex_wires },
    { "password", app_password },
    { NULL, NULL }
};

#endif