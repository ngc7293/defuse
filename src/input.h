#ifndef INPUT_H_
#define INPUT_H_

#include <stdlib.h>

char prompt_char(const char* display, const char* accept, int canon);
void prompt_line(const char* display, char* dest, size_t length);

#endif