#include <iostream>
#include <fstream>
#include "File.h"
#include "StringFn.h"

using namespace std;

const char *logsFile = "keywordIndex.txt";
const char *visitedFile = "visited.txt";

char *readFile(char *filepath)
{
    ifstream file(filepath, ios::in | ios::binary);
    if (!file.is_open())
    {
        // cout << "File not found" << endl;
        return nullptr;
    }
    file.seekg(0, ios::end);
    size_t len = file.tellg();
    file.seekg(0, ios::beg);
    char *buffer = new char[len + 1];
    file.read(buffer, len);
    buffer[len] = '\0';
    file.close();

    return buffer;
}

void appendKeywordLogs(char *keywordUrlLine) {
    ofstream file(logsFile, ios::app); 
    file << keywordUrlLine;
    file << "\n";
}

char *fetchUrls(char *keyword)
{
    char *buffer = readFile((char *)(logsFile));
    if (!buffer)
        return nullptr;

    size_t bufLen = my_strlen(buffer);
    char *urlList = new char[bufLen]();
    size_t i = 0;

    char *searchPtr = buffer;
    // while ((searchPtr = my_strcasestr(searchPtr, keyword)) != nullptr)
    // {
    //     char *keywordLine = my_strcasestr(searchPtr, keyword);
    //     if (!keywordLine)
    //         break;

    //     size_t keywordLen = my_strlen(keyword);
    //     keywordLine += keywordLen + 1;

    //     char *dash = keywordLine;
    //     // while (*dash != '\0' && *dash != '\n' && *dash != '-')
    //     //     dash++;
    //     if (*dash != '-')
    //     {
    //         searchPtr = keywordLine + 1;
    //         continue;
    //     }

    //     char *urlStart = dash + 1;
    //     while (*urlStart && isspace((unsigned char)*urlStart))
    //         urlStart++;

    //     char *urlEnd = my_strcasestr(urlStart, "\n");
    //     if (!urlEnd)
    //         urlEnd = urlStart + my_strlen(urlStart);

    //     char *urlEndTrim = urlEnd;
    //     while (urlEndTrim > urlStart && isspace((unsigned char)*(urlEndTrim - 1)))
    //         urlEndTrim--;

    //     size_t len = (size_t)(urlEndTrim - urlStart);
    //     if (len == 0)
    //     {
    //         searchPtr = urlEnd + 1;
    //         continue;
    //     }

    //     if (i + len + 1 >= bufLen)
    //         break;

    //     char* tempMem = new char[len + 1]();
    //     tempMem[0] = '\0';
    //     for(size_t i = 0; i< len; i++) {
    //         tempMem[i] = urlStart[i];
    //     }
    //     tempMem[len] = '\0';

    //     my_strcpy(urlList + i, tempMem);
    //     delete[] tempMem;
    //     i += len;

    //     urlList[i++] = ',';

    //     searchPtr = urlEnd + 1;
    // }

    char* keywordLine = my_strcasestr(searchPtr, keyword);
    if (!keywordLine)
        return nullptr;

    size_t keywordLen = my_strlen(keyword);
    keywordLine += keywordLen + 4;

    char* end = my_strcasestr(keywordLine, "\n");
    if (!end)
        return nullptr;

    size_t len = (size_t)(end - keywordLine);

    for(i = 0; i< len; i++) {
        urlList[i] = keywordLine[i];
    }
    urlList[len] = '\0';

    delete[] buffer;
    return urlList;
}

void visitedLog(char *url) {
    ofstream file(visitedFile, ios::app);
    file << url;
    file << "\n";
    file.close();
}
