#include <iostream>
#include <filesystem>
#include "Crawler.h"
#include "LinkedList.h"
#include "Hash.h"
#include "StringFn.h"
#include "File.h"

using namespace std;
namespace fs = filesystem;

Crawler :: Crawler()
{
    url = nullptr;
    targetDir = nullptr;
    depth = 0;
    duplicateCount = 0;
    urlList = nullptr;
    linkCount = 20;
    currLinkCount = 0;
    urlHash = new Hash<char*, char*>;
}
Crawler :: Crawler(char *url, char *targetDir, int depth = 5, int linkCount = 20)
{
    this->url = url;
    this->targetDir = targetDir;
    this->depth = depth;
    duplicateCount = 0;
    this->linkCount = linkCount;
    currLinkCount = 0;
    urlList = nullptr;
    urlHash = new Hash<char*, char*>;
}

Crawler :: Crawler(const Crawler& cr)
{
    this->url = cr.url;
    this->targetDir = cr.targetDir;
    this->depth = cr.depth;
    this->duplicateCount = cr.duplicateCount;
    this->linkCount = cr.linkCount;
    this->currLinkCount = cr.currLinkCount;
    this->urlList = cr.urlList;
    this->urlHash = cr.urlHash;
}

Crawler ::~Crawler()
{
    delete[] url;
    delete[] targetDir;
    delete[] urlList;
    delete[] urlHash;
}

void Crawler :: crawl(char *url, int currDepth = 1)
{
    if (depth == 0 || currDepth > depth || currLinkCount > linkCount)
    {
        return;
    }
    if (url == nullptr || targetDir == nullptr || depth == 0)
    {
        cout << "provide the seeds for crawling" << endl;
        return;
    }

    char filename[MAX_LEN];
    filename[0] = '\0';
    generateFileName(filename, 12 % 95);

    if (urlHash->search(url) != nullptr)
    {
        cout << "url already crawled : " << url << endl;
        duplicateCount++;
        return;
    }

    if (isValidUrl(url) == 0 )
    {
        char filepath[MAX_LEN];
        filepath[0] = '\0';
        my_strcat(filepath, targetDir);
        my_strcat(filepath, "/");
        my_strcat(filepath, filename);

        checkDir();
        char cmd[MAX_LEN];
        cmd[0] = '\0';
        my_strcat(cmd, "wget -q ");
        my_strcat(cmd, url);
        my_strcat(cmd, " -O ");
        my_strcat(cmd, filepath);
        system(cmd);
        cout << endl << currLinkCount << " " << url << endl << endl;
        currLinkCount++;
        hashUrl(url, filepath);
        insertUrl(url, filepath);
        extractUrls(filepath, depth, currDepth );
    }
    else
    {
        cout << "invalid url : " << url << endl;
    }
}

void Crawler :: checkDir()
{
    if (!fs::exists(targetDir))
    {
        cout << "directory doesn't exist : " << targetDir << endl;
        if (fs::create_directories(targetDir))
        {
            cout << "created directory : " << targetDir << endl;
        }
        else
        {
            cerr << "failed to create directory : " << targetDir << endl;
        }
    }
}

void Crawler :: generateFileName(char *filename, int len)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    filename[0] = '\0';

    for (int i = 0; i < len; i++)
    {
        int index = rand() % (sizeof(charset) - 1);
        filename[i] = charset[index];
    }
    filename[len] = '\0';
    my_strcat(filename, ".html");
}

int Crawler :: isValidUrl(char *url)
{
    char validityCmd[MAX_LEN];
    validityCmd[0] = '\0';
    my_strcat(validityCmd, "wget --spider -q ");
    my_strcat(validityCmd, url);
    int result = system(validityCmd);
    return result;
}

void Crawler :: extractUrls(char *filepath, int depth, int currDepth)
{
    char *buffer = readFile(filepath);

    if (buffer == nullptr)
    {
        cout << "File not found" << endl;
        return;
    }
    char *searchPos = buffer;
    char *ahref = nullptr;
    while ((ahref = my_strcasestr(searchPos, "<a href=\"http")) != nullptr)
    {
        char *quoteStart = my_strstr(ahref, "\"");
        if (quoteStart == nullptr)
            break;

        char *quoteEnd = my_strstr(quoteStart + 1, "\"");
        if (quoteEnd == nullptr)
            break;

        int len = quoteEnd - quoteStart - 1;
        if (len > MAX_LEN)
            len = MAX_LEN - 1;
        char url[MAX_LEN];
        int i = 0;
        while (i < len)
        {
            url[i] = quoteStart[i + 1];
            i++;
        }
        url[len] = '\0';

        if(currDepth < depth){
            crawl(url, currDepth + 1);
        }

        searchPos = quoteEnd + 1;
    }
    delete buffer;
}

void Crawler :: insertUrl(char *url, char *filepath)
{
    
    if (urlList == nullptr)
    {
        urlList = new LinkedList<char *, char *>();
    }
    char *urlCopy = new char[my_strlen(url) + 1];
    my_strcpy(urlCopy, url);
    char *keyCopy = new char[my_strlen(filepath) + 1];
    my_strcpy(keyCopy, filepath);
    urlList->append(urlCopy, keyCopy);
}

void Crawler :: hashUrl(char *url, char* filepath) {
    char *urlCopy = new char[my_strlen(url) + 1];
    my_strcpy(urlCopy, url);
    char *fileCopy = new char[my_strlen(filepath) + 1];
    my_strcpy(fileCopy, filepath);
    urlHash->insert(urlCopy, fileCopy);
}

void Crawler :: printUrls()
{
    urlList->print();
}
