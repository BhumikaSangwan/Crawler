#include <iostream>
#include "StringFn.h"
 
using namespace std;

size_t my_strlen(const char *s) {
    size_t len = 0;
    while(s[len] != '\0'){
        len++;
    }
    return len;
}

int my_strcmp(const char* str1, const char* str2) {
    int i = 0;
    while(str1[i] != '\0' && str2[i] != '\0') {
        if(str1[i] != str2[i]) {
            return (str1[i] > str2[i] ? 1 : -1);
        }
        i++;
    }
    return 0;
}

char* my_strcpy(char* dest, const char* src) {
    int i = 0;
    while(src[i] != '\0'){
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}

char* my_strcat(char *dest, const char *src) {
    int i = 0;
    while(dest[i] != '\0') {
        i++;
    }

    int j = 0;
    while(src[j] != '\0') {
        dest[i++] = src[j++];
    }
    dest[i] = '\0';
    return dest;
}

char* my_strchr(const char *s, int c) {
    int i = 0;
    char* str = (char*)s;
    while(str[i] != '\0') {
        if(str[i] == c){
            return str+i;
        }
        i++;
    }
    return nullptr;
}

char* my_strstr(const char *haystack, const char *needle) {
    int i = 0;
    while(haystack[i] != '\0') {
        if(haystack[i] == needle[0]){
            int j = 0;
            while(haystack[i+j] == needle[j] && haystack[i+j] != '\0' && needle[j] != '\0') {
                j++;
            }
            if(needle[j] == '\0') {
                return (char*)(haystack+i);
            }
        }
        i++;
    }
    return nullptr;
}

void reverse_inplace(char *s) {
    size_t i = 0, j = my_strlen(s) - 1;
    while (i < j) {
        char temp = s[i];
        s[i] = s[j];
        s[j] = temp;
        i++;
        j--;
    }
}

char toLowerCase(char ch) {
    if (ch >= 'A' && ch <= 'Z') {
        ch += 'a' - 'A';
    }
    return ch;
}

int is_palindrome(const char *s) {
    size_t i = 0, j = my_strlen(s) - 1;
    while(i < j) {
        if(toLowerCase(s[i]) != toLowerCase(s[j])) {
            return 0;
        }
        i++;
        j--;
    }
    return 1;
}

int count_words(const char *s) {
    int words = 0;
    int i = 0;
    char prev = s[i++];
    while(s[i] != '\0') {
        if(s[i] == '\t' || s[i] == '\n') {
            if(prev != ' ' && prev != '\t' && prev != '\n') {
                words++;
            }
            prev = s[++i];
            i++;
            continue;
        }
        if(s[i] == ' ' && prev != ' ') {
            words++;
        }
        prev = s[i++];
    }
    if(s[i-1] != ' ' && s[i-1] != '\n' && s[i-1] != '\t') {
        words++;
    }
    return words;
}

void normalizeSpace(char *str) {
    int i = 0;
    int j = 0;
    char prev = str[i++];
    if(prev != ' ' || prev != '\t' || prev != '\n') {
        str[j++] = prev;
    }
    while(str[i] != '\0') {
        if(str[i] != ' ' && str[i] != '\t' && str[i] != '\n') {
            str[j++] = str[i++];
            continue;   
        }
        if(str[i] == ' ' && str[j-1] != ' '){
            str[j++] = ' ';
        }
        prev = str[i++];
        
    }
    str[j] = '\0';
}

char* my_strcasestr(const char *haystack, const char *needle) {
    int i = 0;
    while(haystack[i] != '\0') {
        if(toLowerCase(haystack[i]) == toLowerCase(needle[0])) {
            int j = 0;
            while(toLowerCase(haystack[i+j]) == toLowerCase(needle[j]) && haystack[i+j] != '\0' && needle[j] != '\0') {
                j++;
            }
            if(needle[j] == '\0') {
                return (char*)(haystack+i);
            }
        }
        i++;
    }
    return nullptr;
}

char* my_strtok(char *str, const char *delim) {
    static char *last = str;
    static char *token = str;
    static char *delimit = (char*)delim;
    if(*last == '\0') {
        return nullptr;
    }
     
    while(*token != '\0') {
        char* delimiter = my_strchr(token, *delimit);
        if(delimiter != nullptr) {
            *delimiter = '\0';
            last = token;
            token = delimiter + 1;
            return last;
        }
        token++;
    }
    *last = '\0';
    return last;
}

void templateReplacer(char *str, struct keyValue replacement) {
    char placeholderStart[] = "{{";
    char placeholderEnd[] = "}}";

    char keyBuffer[100];
    keyBuffer[0] = '\0';
    my_strcat(keyBuffer, placeholderStart);
    my_strcat(keyBuffer, replacement.key);
    my_strcat(keyBuffer, placeholderEnd);
    char* keyStart = my_strstr(str, keyBuffer);
    if(keyStart == nullptr) {
        cout << "key doesn't exist : " << keyBuffer << endl;
        return;
    }
    my_strReplace(str, keyBuffer, replacement.value, keyStart);
}

void shiftLeft(char *str, size_t len) {
    size_t i = 0;
    while(str[i] != '\0' && str[i + len] != '\0') {
        str[i] = str[i+len];
        i++;
    }
    str[i] = '\0';
}

void shiftRight(char* str, size_t inc) {
    size_t i = 0;
    size_t len = my_strlen(str);

    while(len > 0) {
        str[len+inc] = str[len];
        len--;
    }
}

void my_strReplace(char *str, char *key, char *value, char* keyStart) {
    size_t keyLen = my_strlen(key);
    size_t valueLen = my_strlen(value);

    char* valueStart = keyStart;
    
    if(keyLen > valueLen) {
        size_t diff = keyLen - valueLen;
        shiftLeft(keyStart , keyLen - valueLen);
    } else if(keyLen < valueLen) {
        shiftRight(keyStart + keyLen, valueLen - keyLen);
    }

    size_t i = 0;
    while(i<valueLen) {
        valueStart[i] = value[i];
        i++;
    }
}