#ifndef CRAWLER_H
#define CRAWLER_H

#include "LinkedList.h"
#include "Hash.h"

class Crawler {
    private : 
        char* url;
        char* targetDir;
        int duplicateCount;
        int depth;
        int linkCount;
        int currLinkCount;
        LinkedList<char*, char*>* urlList;
        Hash<char*, char*>* urlHash;
    public : 
        Crawler();
        Crawler(char* url, char* targetDir, int depth, int linkCount);
        Crawler(const Crawler& cr);
        ~Crawler();

        void generateFileName(char* filename, int len);
        void insertUrl(char* url, char* filepath);
        void hashUrl(char* url, char* filepath);
        void checkDir();
        int isValidUrl(char* url);
        void crawl(char* url, int currDepth);
        void extractUrls(char* filename, int depth, int currDepth = 1);  
        void printUrls();
};

#endif