#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <termios.h>
#include <unistd.h>

#include "bomb.h"
#include "input.h"

struct wire {
    int has_red;
    int has_blue;
    int has_star;
    int has_led;
    char action;
} wires[16];

/** is_comment
 * @param line a string
 * @return 1 if line starts with '#' (ignoring whitespace), 0 otherwise
 */
static int is_comment(char* line)
{
    int i;
    for (i = 0; line[i]; i++) {
        if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n') {
            return (line[i] == '#');
        }
    }
    return 0;
}

/** load
 * Fills the wires array with the correct conditions/actions. Reads data from
 * `data/complex_wires.conf`, see file for more details on syntax.
 *
 * @return 1 on success, 0 otherwise
 */
static int load()
{
    char line[128];
    FILE* file = fopen("data/complex_wires.conf", "r");

    if (!file) {
        return 0;
    }

    int i = 0;
    while (fgets(line, 128, file) != NULL) {
        if (is_comment(line)) {
            continue;
        }

        if (sscanf(line, "%d %d %d %d %c", &wires[i].has_red, &wires[i].has_blue, &wires[i].has_star, &wires[i].has_led, &wires[i].action) == EOF) {
            continue;
        }

        i++;
    }

    fclose(file);
    return (i == 16);
}

/** condition_cut
 * Check a condition and cut if true. Prompts user for condition value if it is
 * not known (-1).
 *
 * @param cond user-readable condition string (fed to prompt)
 * @param var  container for condition value
 */
static void condition_cut(const char* cond, int* var)
{
    if (*var == -1) {
        *var = (prompt_char(cond, "yn", 0) == 'y');
    }

    if (*var == 1) {
        puts("Cut");
    } else {
        puts("Do not cut");
    }
}

/** app_complex_wires
 * Application function for the Complex Wires/Wire 2 module. Simply reads the
 * wire description from STDIN and tells user to cut or not. Prompts user for
 * extra conditions (serial/parallel port/batteries) if needed, but remembers
 * the reply (only asks once).
 *
 * @param bomb the current bomb. Will check/prompt for serial number, parallel
 *             port & battery count
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 *
 * @note This temporarily unsets ICANON for STDIN. Ctrl-C-ing here might leave
 *       the tty in a unexpected state.
 */
int app_complex_wires(struct bomb* bomb)
{
    char c;
    struct termios config;

    int serial = -1;
    int parallel = -1;

    if (!load()) {
        return EXIT_FAILURE;
    }

    while (1) {
        struct wire current;
        current.has_red =  (prompt_char("Red ", "yn", 0) == 'y');
        current.has_blue = (prompt_char("Blue", "yn", 0) == 'y');
        current.has_star = (prompt_char("Star", "yn", 0) == 'y');
        current.has_led =  (prompt_char("LED ", "yn", 0) == 'y');

        char action;
        int i;
        for (i = 0; i < 16; i++) {
            if (memcmp(&current, &wires[i], 4 * sizeof(int)) == 0) {
                action = wires[i].action;
                break;
            }
        }

        switch (action) {
            case 'C':
                puts("Cut");
                break;
            case 'D':
                puts("Do not cut");
                break;
            case 'S':
                condition_cut("Is the last digit of the serial number even?", &serial);
                break;
            case 'P':
                condition_cut("Does the bomb have a parallel port?", &parallel);
                break;
            case 'B':
                if (bomb->batteries == -1) {
                    bomb->batteries = prompt_char("How many batteries does the bomb have?", "0123456789", 1) - '0';
                }
                if (bomb->batteries >= 2) {
                    puts("Cut");
                } else {
                    puts("Do not cut");
                }
                break;
        }

        if (prompt_char("Again?", "yn", 0) == 'n') {
            break;
        }
    }

    return EXIT_SUCCESS;
}