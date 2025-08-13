#ifndef FILE_H
#define FILE_H

extern const char* logsFile;
extern const char* visitedFile;

char* readFile(char* filepath);
char* fetchUrls(char* keyword);
void appendKeywordLogs(char* keywordUrlLine);
void visitedLog(char* url);

#endif