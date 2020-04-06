#ifndef INPUT_H_
#define INPUT_H_

#include <stdlib.h>

void canon_tty();
void uncanon_tty();

char prompt_char(const char* display, const char* accept, int canon);

int prompt_yesno(const char* display);
int prompt_digit(const char* display, int min, int max);

char* prompt_line(const char* display, char* dest, size_t length);

#endif