int contains(char* str, char c)
{
    int i = 0;
    for (; str[i] != 0; i++) {
        if (str[i] == c) {
            return 1;
        }
    }

    return 0;
}

int is_comment(char* line)
{
    int i;
    for (i = 0; line[i]; i++) {
        if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n') {
            return (line[i] == '#');
        }
    }
    return 0;
}
