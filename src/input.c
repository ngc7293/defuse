#include "input.h"

#include <stdio.h>
#include <stdlib.h>

#include <termios.h>
#include <unistd.h>

static void canon_tty()
{
    struct termios config;
    tcgetattr(STDIN_FILENO, &config);
    config.c_lflag |= ICANON;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &config);
}

static void uncanon_tty()
{
    struct termios config;
    tcgetattr(STDIN_FILENO, &config);
    config.c_lflag &= ~(ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &config);
}

char prompt_char(const char* display, const char* accept, int canon)
{
    char c;
    int i;

    if (canon == 0) {
        uncanon_tty();
    }

    while (1) {
        printf("%s [%s]: ", display, accept);
        c = fgetc(stdin);

        if (canon) {
            int t;
            while ((t = getchar()) != '\n' && t != EOF);  // Consume what's left in input buffer.
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
}

void prompt_line(const char* display, char* dest, size_t length)
{
}
