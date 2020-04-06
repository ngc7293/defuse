#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <signal.h>
#include <pthread.h>

#include "apps.h"
#include "bomb.h"

static pthread_t* current_thread;

/** app_help
 * Prints all available functions
 * @param bomb (unused)
 * @return EXIT_SUCCESS
 */
int app_help(struct bomb* bomb)
{
    for (int i = 0; apps[i].name != NULL; i++) {
        puts(apps[i].name);
    }

    return EXIT_SUCCESS;
}

/** sigint_thread_handler
 * Cancels current thread
 *
 * @param signum (unused)
 */
void sigint_thread_handler(int signum)
{
    pthread_cancel(*current_thread);
    putc('\n', stdout);
}

/** thread_run
 * Entry function for "application" (module) threads. Provides them with a
 * simpler call  signature (int return code, bomb* argument).
 *
 * @param arguments the argument structure passed to pthread_create. MUST MATCH
 *                  the structure created in `run` EXACTLY.
 */
void* thread_run(void* arguments)
{
    struct {
        int* rc;
        int (*function)(struct bomb*);
        struct bomb *bomb;
    } *args = arguments;

    *(args->rc) = args->function(args->bomb);
    return NULL;
}

/** run
 * Run an application function in a subthread and returns with the function's
 * exit code.
 *
 * @param name application function's name
 * @param bomb bomb struct passed to application
 *
 * @return the applications exit code, or EXIT_FAILURE if none matched name.
 */
int run(char* name, struct bomb* bomb)
{
    int i;
    int (*function)(struct bomb*) = NULL;
    pthread_t thread;

    for (i = 0; apps[i].name != NULL; i++) {
        if (strcmp(name, apps[i].name) == 0) {
            function = apps[i].function;
            break;
        }
    }

    if (function) {
        int rc = EXIT_FAILURE;
        struct {
            int* rc;
            int (*function)(struct bomb*);
            struct bomb* bomb;
        } args = { &rc, function, bomb };

        pthread_create(&thread, NULL, thread_run, (void*)&args);
        current_thread = &thread;
        signal(SIGINT, sigint_thread_handler);

        pthread_join(thread, NULL);
        current_thread = NULL;
        signal(SIGINT, SIG_DFL);

        return rc;
    }
    else {
        return EXIT_FAILURE;
    }
}

int main(int argc, const char* argv[])
{
    char line[128];
    char *tok;
    int rc = 0;

    struct bomb bomb;
    bomb.batteries = -1;

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

        if (strcmp(line, "exit") == 0) {
            break;
        }

        rc = run(line, &bomb);
    }

    return EXIT_SUCCESS;
}