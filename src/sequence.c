#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bomb.h"
#include "input.h"
#include "util.h"

#define RED   0
#define BLUE  1
#define BLACK 2

static int strtocol(char* str)
{
    if (strcmp(str, "red") == 0) {
        return RED;
    } else if (strcmp(str, "blue") == 0) {
        return BLUE;
    } else if (strcmp(str, "black") == 0) {
        return BLACK;
    }

    return -1;
}

static char* get_section(char* line)
{
    int i = 0;
    char* start = NULL;

    for (; line[i]; i++) {
        if (line[i] == ' ' || line[i] == '\t') {
            continue;
        }

        if (line[i] == '[') {
            if (!start) {
                start = &line[i + 1];
            } else {
                return NULL;
            }
        }

        if (line[i] == ']') {
            if (start) {
                line[i] = 0;
                return start;
            } else {
                return NULL;
            }
        }
    }

    return NULL;
}

static int load(char config[3][9][4])
{
    char line[128];
    char *section;

    int current = -1;
    int index = 0;

    FILE* file;


    if (!(file = fopen("data/sequence.conf", "r"))) {
        return 0;
    }

    while (fgets(line, 128, file) != NULL) {
        if (is_comment(line)) {
            continue;
        }

        section = get_section(line);
        if (section) {
            current = strtocol(section);
            index = 0;
            continue;
        }

        if (current == -1 || index > 9) {
            continue;
        }

        if (sscanf(line, "%3[abc]", config[current][index]) == EOF) {
            continue;
        }

        index++;
    }

    fclose(file);
    return 1;
}

int app_sequence(bomb_t* bomb)
{
    char config[3][9][4];
    int count[3] = { 0, 0, 0};
    int current;

    char line[128];
    char color[16];
    char letter;

    if (!load(config)) {
        return EXIT_FAILURE;
    }

    puts("For each wire, type '<color> <letter>'. e.g.: 'black a'");
    puts("Empty line to exit.");

    while (1) {
        fputs("Color: ", stdout);
        fgets(line, 128, stdin);

        if (line[0] == '\n') {
            break;
        }

        if (sscanf(line, "%s %1[abc]", color, &letter) != 2) {
            puts("Invalid syntax");
            continue;
        }

        if ((current = strtocol(color)) == -1 || count[current] >= 9) {
            puts("Invalid color");
            continue;
        }

        if (contains(config[current][count[current]++], letter)) {
            puts("Cut");
        } else {
            puts("Do not cut");
        }
    }

    return EXIT_SUCCESS;
}
