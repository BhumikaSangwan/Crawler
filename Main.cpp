#include <iostream>
#include "Crawler.h"
#include "StringFn.h"
#include "SearchEngine.h"

using namespace std;

void findKeyword();
void crawlUrl(char *, char *, int, int);

int main(int argc, char *argv[])
{
    srand(time(0));
    char *url = argv[1];
    char *targetDir = argv[2];
    int depth = atoi(argv[3]);
    int linkCount = atoi(argv[4]);
    SearchEngine se;
    if (se.isUrlCrawled(argv[1]))
    {
        cout << "URL already crawled." << endl;

        cout << "Do you want to re-crawl (y/n) : ";
        char ch;
        cin >> ch;
        if (toLowerCase(ch) == 'y')
        {
            crawlUrl(url, targetDir, depth, linkCount);
        }
        else
        {
            findKeyword();
        }
    }
    else
    {
        crawlUrl(url, targetDir, depth, linkCount);
    }

    return 0;
}

void findKeyword()
{
    cout << "Enter the keyword to search : ";
    char keyword[100];
    cin >> keyword;
    SearchEngine se1;
    se1.searchKeyword(keyword);
}

void crawlUrl(char *url, char *targetDir, int depth, int linkCount)
{
    Crawler cr(url, targetDir, depth, linkCount);
    cr.crawl(url, 0); // url, currDepth
    cr.findKeywords();
}