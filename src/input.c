#include "input.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <termios.h>
#include <unistd.h>

static void clear_stdin()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);  // Consume what's left in input buffer.
}

void canon_tty()
{
    struct termios config;
    tcgetattr(STDIN_FILENO, &config);
    config.c_lflag |= ICANON;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &config);
}

void uncanon_tty()
{
    struct termios config;
    tcgetattr(STDIN_FILENO, &config);
    config.c_lflag &= ~(ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &config);
}

static char prompt_char_internal(const char* display, const char* accept, const char* display_accept, int canon)
{
    int c = 0;
    int i;

    if (canon == 0) {
        uncanon_tty();
    }

    while (c != EOF) {
        printf("%s [%s]: ", display, display_accept);
        c = fgetc(stdin);

        if (canon) {
            clear_stdin();
        } else {
            putc('\n', stdout);

        }

        for (i = 0; accept[i]; i++) {
            if (accept[i] == c) {
                canon_tty();
                return c;
            }
        }
    }

    return 0;
}

char prompt_char(const char* display, const char* accept, int canon)
{
    return prompt_char_internal(display, accept, accept, canon);
}

int prompt_yesno(const char* display)
{
    return (prompt_char(display, "yn", 0) == 'y');
}

int prompt_digit(const char* display, int min, int max)
{
    char range[11] = { 0 };
    char display_range[4] = { (min + '0'), '-', (max + '0'), 0 };
    int i = 0;

    for (; i <= (max - min); i++) {
        range[i] = (i + min) + ('0');
    }

    return prompt_char_internal(display, range, display_range, 1) - '0';
}

char* prompt_line(const char* display, char* dest, size_t length)
{
    char* rc;
    size_t last;

    while (1) {
        
        fputs(display, stdout);
        putc(' ', stdout);
        rc = fgets(dest, length, stdin);
        last = strlen(dest) - 1;

        if (dest[last] == '\n') {
            dest[last--] = 0;
        }

        if (last > 0 || rc == NULL) {
            break;
        }
    }

    return rc;
}
