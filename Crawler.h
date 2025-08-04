#ifndef CRAWLER_H
#define CRAWLER_H

class Crawler {
    private : 
        char* url;
        char* targetDir;
        int depth;
    public : 
        Crawler();
        Crawler(char* url, char* targetDir, int depth);
        ~Crawler();

        void generateFileName(char* filename, int len);
        void checkDir();
        int isValidUrl(char* url);
        void crawl(char* url, int depth);
        void extractUrls(char* filename);  
};

#endif