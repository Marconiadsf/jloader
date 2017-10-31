#ifdef _NC_RESTRICT
char *strcpy(char *restrict dest, const char *restrict src)
#else
char *strcpy(char *dest, const char* src)
#endif
{
    char *ret = dest;
    while (*dest++ = *src++)
        ;
    return ret;
}
