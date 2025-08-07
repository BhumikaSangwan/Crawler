#ifndef STRING_FN_H
#define STRING_FN_H
#define MAX_SIZE 30
#define MAX_LEN 100

struct keyValue{
    char key[MAX_SIZE];
    char value[MAX_SIZE];
};

size_t my_strlen(const char* str);
int my_strcmp(const char* str1, const char* str2);
char* my_strcpy(char* dest, const char* src);
char* my_strcat(char *dest, const char *src);
char* my_strchr(const char *s, int c);
char* my_strstr(const char *haystack, const char *needle);
char* my_strtok(char *str, const char *delim);
void my_strReplace(char *str, char *key, char *value, char* keyStart);
void my_strLowerCase(char *str);


char toLowerCase(char ch);

void reverse_inplace(char *s);
int is_palindrome(const char *s);
int count_words(const char *s);
void normalizeSpace(char *s);
char* my_strcasestr(const char *haystack, const char *needle);
void templateReplacer(char *str, struct keyValue replacement);


#endif