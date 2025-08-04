#include <iostream>
#include <filesystem>
#include "Crawler.h"
#include "LinkedList.h"
#include "Hash.h"
#include "StringFn.h"

using namespace std;
namespace fs = filesystem;

Crawler ::Crawler()
{
    url = nullptr;
    targetDir = nullptr;
    depth = 0;
}
Crawler ::Crawler(char *url, char *targetDir, int depth)
{
    this->url = url;
    this->targetDir = targetDir;
    this->depth = depth;
}

Crawler ::~Crawler()
{
    delete[] url;
    delete[] targetDir;
}

// void Crawler :: createLL() {
//     LinkedList<string, string> ll;
//     ll.prepend("zero", "zero");
//     ll.insert("1", "1", 0);
//     ll.prepend("two", "two");
//     ll.append("4", "random");
//     ll.prepend("zro", "zero");
//     ll.append("four", "four");
//     ll.prepend("5", "five");
//     ll.append("six", "six");
//     ll.append("svn", "seven");
//     ll.print();
// }

void Crawler :: crawl(char *url, int depth)
{
    if (url == nullptr || targetDir == nullptr || depth == 0)
    {
        cout << "provide the seeds for crawling" << endl;
        return;
    }

    cout << "crawling started" << endl;

    char filename[MAX_LEN];
    filename[0] = '\0';
    generateFileName(filename, 12%95);
    cout << "filename : " << filename << endl;

    if ( isValidUrl(url) == 0)
    {
        checkDir();
        char cmd[MAX_LEN];
        cmd[0] = '\0';
        my_strcat(cmd, "wget -q ");
        my_strcat(cmd, url);
        my_strcat(cmd, " -O ");
        my_strcat(cmd, targetDir);
        my_strcat(cmd, "/");
        my_strcat(cmd, filename);
        system(cmd);
        cout << "wget success" << endl;
        extractUrls(my_strcat(my_strcat(targetDir, "/"), filename));
    }
    else
    {
        cout << "invalid url" << endl;
    }
}

void Crawler ::checkDir()
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

void Crawler :: extractUrls(char *filepath) {
    cout << "filePath : " << filepath << endl; 

}