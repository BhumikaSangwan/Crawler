#ifndef CRAWLER_H
#define CRAWLER_H

#include "LinkedList.h"
#include "Hash.h"

class Crawler
{
private:
    char *url;
    char *targetDir;
    int duplicateCount;
    int depth;
    int linkCount;
    int currLinkCount;
    LinkedList<char *, char *> *urlList;
    Hash<char *, char *> *urlHash;

public:
    Crawler();
    Crawler(char *url, char *targetDir, int depth, int linkCount);
    Crawler(const Crawler &cr);
    ~Crawler();

    void generateFileName(char *filename, int len);
    void insertUrl(char *url, char *filepath);
    void getUrl(char *url, int len, int currDepth, char *parentUrl);
    void hashUrl(char *url, char *filepath);
    void checkDir();
    bool isHtml(char *url);
    int isValidUrl(char *url);
    void extractUrls(char *filename, int currDepth, char *parentUrl);
    void printUrls();
    void printHashedUrls();
    bool isValidWord(char *word);
    char *getWord(char *&searchPos);
    char *getKeyword(char *filepath, char *url);
    void findKeywords();
    void crawl(char *url, int currDepth);
};

#endif