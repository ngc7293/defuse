#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "apps.h"

#include <stdio.h>
#include <stdlib.h>

/** app_help
 * Prints all available functions
 * @param args (unused)
 * @return EXIT_SUCCESS
 */
int app_help(char* args[])
{
    int i = 0;

    while (apps[i].name != NULL) {
        puts(apps[i].name);
        i++;
    }

    return EXIT_SUCCESS;
}

/** app_exit
 * Exits with code EXIT_SUCCESS.
 * @param args
 */
int app_exit(char* args[])
{
    exit(EXIT_SUCCESS);
}

int main(int argc, const char* argv[])
{
    char line[128];
    char *tok;
    int (*function)(char*[]);
    int rc = 0, i;

    while (1) {
        if (rc != EXIT_SUCCESS) {
            fputs("\x1b[31m!\x1b[0m", stdout);
        }
        fputs("$ ", stdout);

        if (!fgets(line, 128, stdin)) {
            puts("exit");
            return EXIT_SUCCESS;
        }

        if ((tok = strtok(line, "\n\t ")) == NULL) {
            continue;
        }

        function = NULL;
        for (i = 0; apps[i].name != NULL; i++) {
            if (strcmp(line, apps[i].name) == 0) {
                function = apps[i].function;
                break;
            }
        }

        if (function) {
            rc = function(NULL);
        } else {
            rc = -1;
        }
    }

    return EXIT_SUCCESS;
}