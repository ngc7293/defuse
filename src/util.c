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