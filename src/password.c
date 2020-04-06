#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>
#include <pthread.h>

#include "bomb.h"
#include "util.h"

struct word {
    char text[6];
    struct word* next;
};

static struct word* head;

/** load
 * Loads words from a conf file into a linked-list.
 * Reads file `data/password`, ignoring comments, empty line or invalid input,
 * and copies words into a malloc'd linked-list. Allocated struct words should
 * be freed, or unload() should be called when they are no longer needed.
 *
 * @return head of (single) linked-list of struct text.
 */
static struct word *load()
{
    head = NULL;
    struct word *current = head;

    char line[128];

    FILE* file = fopen("data/password.conf", "r");

    if (!file) {
        return NULL;
    }

    while (fgets(line, 128, file) != NULL) {
        if ((sscanf(line, "%s", line) == EOF) || line[0] == '#' || strlen(line) != 5) {
            continue;
        }

        if (!head) {
            current = head = (struct word*)malloc(sizeof(struct word));
        } else {
            current -> next = (struct word*)malloc(sizeof(struct word));
            current = current -> next;
        }

        strcpy(current -> text, line);
        current -> next = NULL;
    }

    fclose(file);
    return head;
}

/** unload
 * Frees all manually allocated structures in linked-list
 *
 * @param head start of linked-list of struct word
 */
static void unload(void* args)
{
    struct word *current = head;
    struct word *temp;

    while (current) {
        temp = current;
        current = current -> next;
        free(temp);
    }

    head = NULL;
}

/** app_password
 * Application function for the Password (5-digit input) module.
 * The basic logic here is to load all the words into a linked-list, and remove
 * them if the letter at position n isn't available on the input. When you only
 * have a single value left, you've found your password.
 *
 * @param args (unused)
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE.
 */
int app_password(bomb_t *bomb)
{
    static const char* positions[] = { "first", "second", "third", "fourth", "fifth" };

    struct word *current, *previous, *next;
    char line[128];
    int pos;

    if ((head = load()) == NULL) {
        return EXIT_FAILURE;
    }
    pthread_cleanup_push(unload, NULL);

    for (pos = 0; pos < 5; pos++) {
        printf("%s position: ", positions[pos]);
        if (!fgets(line, 128, stdin)) {
            return EXIT_FAILURE;
        }

        current = head;
        while (current) {
            if (!contains(line, current -> text[pos])) {
                if (current == head) {
                    head = current->next;
                } else {
                    previous->next = current -> next;
                }

                next = current -> next;
                free(current);
            } else {
                previous = current;
                next = current -> next;
            }

            current = next;
        }

        if (head == NULL) {
            puts("No possible matches.");
            return EXIT_FAILURE;  // Nothing to unload anyway
        }

        if (head->next == NULL) {
            printf("password: %s\n", head -> text);
            break;
        }
    }

    pthread_cleanup_pop(1);
    return (pos < 5 ? EXIT_SUCCESS : EXIT_FAILURE);  // Broke out of for-loop
}