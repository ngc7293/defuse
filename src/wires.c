
#include <stdio.h>

#include "bomb.h"
#include "input.h"

static int wires_3(bomb_t *bomb)
{
    char c;

    if (!prompt_yesno("Are there \x1b[31mred\x1b[0m wires?")) {
        puts("Cut the second wire");
    }
    else if (prompt_yesno("Is the last wire white?")) {
        puts("Cut the last wire");
    }
    else if (prompt_yesno("Is there more than one \x1b[34mblue\x1b[0m wire?")) {
        puts("Cut the last \x1b[34mblue\x1b[0m wire");
    } else {
        puts("Cut the last wire");
    }

    return 0;
}

static int wires_4(bomb_t *bomb)
{
    if (prompt_yesno("Is there more than one \x1b[31mred\x1b[0m wire?") && bomb_serial_ends_odd(bomb)) {
        puts("Cut the last \x1b[31mred\x1b[0m wire");
    }
    else if (prompt_yesno("Is the last wire \x1b[33myellow\x1b[0m?") && !prompt_yesno("Are the \x1b[31mred\x1b[0m wires?")) {
        puts("Cut the first wire");
    }
    else if (prompt_yesno("Is there exactly one \x1b[34mblue\x1b[0m wire?")) {
        puts("Cut the first wire");
    }
    else if (prompt_yesno("Is there more than one \x1b[33myellow\x1b[0m wire")) {
        puts("Cut the last wire");
    } else {
        puts("Cut the second wire");
    }

    return 0;
}

static int wires_5(bomb_t *bomb)
{
    if (prompt_yesno("Is the last wire black?") && bomb_serial_ends_odd(bomb)) {
        puts("Cut the fourth wire");
    }
    else if (prompt_yesno("Is there exactly one \x1b[31mred\x1b[0m wire") && prompt_yesno("Is there more than one \x1b[33myellow\x1b[0m wire")) {
        puts("Cut the first wire");
    }
    else if (!prompt_yesno("Are there black wires?")) {
        puts("Cut the second wire");
    } else {
        puts("Cut the first wire");
    }

    return 0;
}

static int wires_6(bomb_t *bomb)
{
    int yellow;
    if ((yellow = prompt_digit("How many \x1b[33myellow\x1b[0m wires?", 0, 6)) == 0 && bomb_serial_ends_odd(bomb)) {
        puts("Cut the third wire");
    }
    else if (yellow == 1 && prompt_yesno("Is there more than one white wire")) {
        puts("Cut the fourth wire");
    }
    else if (!prompt_yesno("Are there \x1b[31mred\x1b[0m wires?")) {
        puts("Cut the last wire");
    } else {
        puts("Cut the fourth wire");
    }

    return 0;
}

int app_wires(bomb_t *bomb)
{
    static const int(*solver[4])(bomb_t*) = {
        wires_3,
        wires_4,
        wires_5,
        wires_6
    };

    int count = (prompt_digit("How many wires?", 3, 6) - 3);
    return (count < 0) ? EXIT_FAILURE : solver[count](bomb);
}
