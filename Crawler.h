#ifndef CRAWLER_H
#define CRAWLER_H

#include "LinkedList.h"

class Crawler {
    private : 
        char* url;
        char* targetDir;
        int depth;
        int linkCount;
        LinkedList<char*, char*>* urlList;
    public : 
        Crawler();
        Crawler(char* url, char* targetDir, int depth, int linkCount);
        ~Crawler();

        void generateFileName(char* filename, int len);
        void insertUrl(char* url, char* filepath);
        void checkDir();
        int isValidUrl(char* url);
        void crawl(char* url, int depth, int currDepth);
        void extractUrls(char* filename, int depth, int currDepth = 1);  
        void printUrls();
};

#endif