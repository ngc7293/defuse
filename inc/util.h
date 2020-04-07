#ifndef UTIL_H_
#define UTIL_H_

/** contains
 * @param str haystack
 * @param c   needle
 * @return 1 if c is in str, else 0
 */
int contains(char* str, char c);

/** is_comment
 * @param line a string
 * @return 1 if line starts with '#' (ignoring whitespace), 0 otherwise
 */
int is_comment(char* line);

#endif